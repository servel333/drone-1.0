/*
 * bluetooth.c
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

#include <stdbool.h>
#include "bluetooth.h"
#include "asf.h"
#include "debug.h"
#include "strings.h"
#include "watchdog.h"

#define BLUETOOTH_SERIAL               &USARTE0
#define BLUETOOTH_SERIAL_BAUDRATE_FAST 115200
#define BLUETOOTH_SERIAL_CHAR_LENGTH   USART_CHSIZE_8BIT_gc
#define BLUETOOTH_SERIAL_PARITY        USART_PMODE_DISABLED_gc
#define BLUETOOTH_SERIAL_STOP_BIT      false  // false = 1, true = 2

static bool bluetooth_on = false;

void setupBluetoothFast(void)
{
	//sysclk_init();
	const usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = BLUETOOTH_SERIAL_BAUDRATE_FAST,
		.charlength = BLUETOOTH_SERIAL_CHAR_LENGTH,
		.paritytype = BLUETOOTH_SERIAL_PARITY,
		.stopbits = BLUETOOTH_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_E, PR_USART0_bm);
	sendDBGString("BT 1152 INIT: ");
	WatchdogPet();
	if (usart_init_rs232(BLUETOOTH_SERIAL, &USART_SERIAL_OPTIONS)) {
		sendDBGLine(pgm_read_byte(good));
	}
	else {
		sendDBGLine(pgm_read_byte(bad));
	}
}

void sendBTHexPair(unsigned char binaryData)
{
	unsigned char hexPair;
	unsigned char val;

	/* High nibble */
	val = binaryData >> 4;
	if (val > 9) {
		  	usart_putchar(BLUETOOTH_SERIAL, (val + 'A' - 10));
	}
	else
	{
		usart_putchar(BLUETOOTH_SERIAL, (val + '0'));
	}

	/* Low nibble */
	val = binaryData & 0x0F;
	if (val > 9) {
		usart_putchar(BLUETOOTH_SERIAL, (val + 'A' - 10));
	}
	else
	{
		usart_putchar(BLUETOOTH_SERIAL, (val + '0'));
	}

}

void sendBTChar(unsigned char str)
{
	if (hardwareflowcontrol(1000))
		usart_putchar(BLUETOOTH_SERIAL, str);
};

// write out a simple '\0' terminated string
void sendBTString(const char *string)
{
	while(*string != 0)
	sendBTChar(*string++);
};

/**
 * write out a simple '\0' terminated string and print "\r\n" at end
 * Then wait for the "OK" response from the module.
 * \param command A null terminated string command for the BT module.
 * \return True if the response was OK, false otherwise */
bool sendBTCmd(const char *command)
{
	if (hardwareflowcontrol(1000))
	{
		sendBTString(command);
		sendBTChar('\r');
		sendBTChar('\n');
	}
	return false; 
};

void turnBTOn(void)
{
	ioport_set_pin_level(BT_ENABLE_PIN, true); // Power on
	delay_ms(100);
	
	ioport_set_pin_level(BT_RESET_PIN, false); // Release it from reset.
	hardwareflowcontrol(16000); // Allow as long as needed
}

void turnBTOff(void) {

sendDBGLine("BTOff");
	
	hardwareflowcontrol(2000);
	sendBTCmd("BC:DC");   // Bluetooth disconnect  
	
	ioport_set_pin_level(BT_RESET_PIN, true); // Hold it in reset so next power on is clean.
	ioport_set_pin_level(BT_ENABLE_PIN, false); // Power off
}

void initBTComms(const char * modeName)
{
	sendDBGLine("initBT");
	wdt_reset();
	
	// Set the Bluetooth Name
	sendBTCmd("BC:NM=Drone");
	
	// Make the module discoverable
	hardwareflowcontrol(2000);
	sendBTCmd("BC:MD=01"); //Make Discoverable. 

	// Configure Bypass mode
	wdt_reset();
	hardwareflowcontrol(1000);
	sendBTCmd("BC:BP=04,07,00"); // Use Binary Mode in Release 	


	// Long delay to let connection happen
	wdt_reset();
	hardwareflowcontrol(6000);
}

void bt_exitBypass(void)
{
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, false); // Pin level is inverted by Transistor Switch
	turnBTOn();
	delay_ms(300);
	
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true); // Pin level is inverted by Transistor Switch
	delay_ms(100);
}

void bt_homeButton(void)
{
	// Send consumer report for home button press
	sendBTChar('\xA1');
	sendBTChar('\x02');
	sendBTChar('\x01');
	sendBTChar('\x00');
	delay_ms(5); 
	
	// Clear consumer report for home button press
	sendBTChar('\xA1');
	sendBTChar('\x02');
	sendBTChar('\x00');
	sendBTChar('\x00');
}

/*
  Manages the RTS/CTS pins to the Bluetooth Module
  if return value = true it's ready to send,
  if return value = false no response. 
*/
bool hardwareflowcontrol(int maxWaitTime) // maxWaitTime in ms
{
	// Will the Bluetooth do this automatically, without the RTS?
	if (ioport_get_pin_level(BT_CTS_PIN)) return true;
	
	// Tell Bluetooth we are "Ready To Send"
	ioport_set_pin_level(BT_RTS_PIN, true);
	bool result = false;
	while(maxWaitTime > 0)
	{
		// See if the Bluetooth is Clear To Send
		result = ioport_get_pin_level(BT_CTS_PIN);
		if (result == true) break;
		maxWaitTime--;
		delay_ms(1);
	}
	return result;
}

void btFactoryReset(void)
{
	turnBTOff();
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, false); // Pin level is inverted by Transistor Switch
	turnBTOn();
	
	delay_ms(350);
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true); // Pin level is inverted by Transistor Switch
}

bool btIsConnected(void)
{
	return !ioport_get_pin_level(BT_STATUS_PIN);// Status Pin is inverted by Transistor Switch
}

void btClearAutoConnectList(void)
{
	hardwareflowcontrol(1000);
	sendBTCmd("BC:CP");
}

bool BluetoothKeyboardMode() // Keyboard (PC & iOS)
{
	return sendBTCmd("BC:CD=000540"); 
}

bool BluetoothMouseMode()
{
	return sendBTCmd("BC:CD=000580");
}

bool BluetoothComboMode() // Keyboard & Mouse
{
	return sendBTCmd("BC:CD=0005C0");
}

bool BluetoothGamePadMode() // Gamepad (default)
{
	return sendBTCmd("BC:CD=000508");
}

bool BluetoothJoystickMode()
{
	return sendBTCmd("BC:CD=0005C4");
}
