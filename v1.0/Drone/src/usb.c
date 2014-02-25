/*
* usb.c
* Version: Drone 1.0
*
* Copyright (C) 2013-2014 EVO CONTROLLERS, LLC.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "usb.h"
#include "asf.h"
#include "debug.h"
#include "circular_buffer.h"
#include "watchdog.h"




static uint8_t rxBuffer[USB_SERIAL_RX_BUFFER_SIZE];
static struct CircularBuffer rxCB;

static uint8_t txBuffer[USB_SERIAL_TX_BUFFER_SIZE];
static struct CircularBuffer txCB;

void setupUsb(void)
{
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USB_SERIAL_BAUDRATE,
		.charlength = USB_SERIAL_CHAR_LENGTH,
		.paritytype = USB_SERIAL_PARITY,
		.stopbits = USB_SERIAL_STOP_BIT
	};
	
	// Enable the clock to this peripheral:
	sysclk_enable_module(SYSCLK_PORT_C, PR_USART0_bm);

	// Initialize this peripheral:
	sendDBGString("USB UART INIT: ");
	WatchdogPet();
	if (usart_init_rs232(USB_SERIAL, &USART_SERIAL_OPTIONS)) {
		sendDBGLine("GOOD");
	}
	else {
		sendDBGLine("BAD");
	}
	
	// Initialize the RX/TX buffer:
	initCB(&rxCB, rxBuffer, USB_SERIAL_RX_BUFFER_SIZE);
	initCB(&txCB, txBuffer, USB_SERIAL_TX_BUFFER_SIZE);
	
	// Enable the rx interrupt handler:
	usart_set_rx_interrupt_level(USB_SERIAL, USART_INT_LVL_LO);
}

void sendUSBChar(unsigned char str)
{
	// Wait for space to be available in the transmit buffer,
	// then add this character:
	while (!addByteToCB(&txCB, str)) ;
	
	// Enable transmission:
	usart_set_dre_interrupt_level(USB_SERIAL, USART_INT_LVL_LO);
}

unsigned char recvUSBChar(void)
{
	uint8_t temp;
	
	// Wait for a byte to be available:
	while (!readByteFromCB(&rxCB, &temp)) ;
	
	return temp;
}

// write out a simple '\0' terminated string
void sendUSBString(const char *string, bool terminate)
{
	while (*string != '\0') {
		sendUSBChar(*string++);
	}
	
	if (terminate) {
		sendUSBChar('\0');
	}
}

// write out a simple '\0' terminated string and print "\n\r" at end
void sendUSBLine(const char *string)
{
	sendUSBString(string, false);
	sendUSBChar('\r');
	sendUSBChar('\n');
}

bool recvUSBString(char* dst, uint8_t max_count) {
	//readStringFromCB(&rxCB, dst, max_count);
	return readStringFromCB(&rxCB, dst, max_count);
}

void FTDIHoldInReset(bool enable) {
	if (enable) {
		ioport_set_pin_level(USB_RESET_PIN, false); // Hold it in reset.
	}
	else {
		ioport_set_pin_level(USB_RESET_PIN, true); // Release it from reset.
	}
}

bool USBPluggedIn(void)
{
	return ioport_get_pin_level(USB_5V_PIN);
}

ISR(USB_SERIAL_RX_INT) {
	char c;
	
	irqflags_t flags;
	flags = cpu_irq_save();	
	
	c = usart_get(USB_SERIAL);
	
	// Insta-reset if this is the break character for
	// entering the bootloader: (Really shouldn't be
	// in the usb code, but it's the quickest)
	if (c == 0x1B) {
		reset_do_soft_reset();
	}
	
	// Add the byte to the buffer. (If it is not full):
	addByteToCB(&rxCB, c);
	
	// Clear the interrupt flag:
	usart_clear_rx_complete(USB_SERIAL);
	
	cpu_irq_restore(flags);
}

ISR(USB_SERIAL_DRE_INT) {
	uint8_t temp;
	
	// Is there any data to send out?
	if (readByteFromCB(&txCB, &temp)) {
		// Then send it:
		usart_put(USB_SERIAL, temp);
	}
	else {
		// Disable the interrupt if there is no more data:
		usart_set_dre_interrupt_level(USB_SERIAL, USART_INT_LVL_OFF);
	}
}

//#if DEBUG

#include <stdlib.h>

void PrintXY(int16_t x, int16_t y)
{
	//string size (NULL char inclusive) for signed 16-bit integer (6+1); rounded up +1 for byte alignment
	#define STR_SIZE_16B 8
	unsigned char numStr[STR_SIZE_16B];
	sendUSBChar( '(' );
	sendUSBString( itoa(x, numStr, 10), false );
	sendUSBChar( ',' );
	sendUSBString( itoa(y, numStr, 10), false );
	sendUSBChar( ')' );
}
//extern int16_t debug_x;
//extern int16_t debug_y;
//#endif
