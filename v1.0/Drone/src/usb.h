/*
 * usb.h
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

#ifndef USB_H_
#define USB_H_

#include <stdint.h>
#include <stdbool.h>
#include "asf.h"

#define USB_SERIAL                     &USARTC0
//#ifdef FIRST_500
  #define USB_SERIAL_BAUDRATE            57600
//#else
//  #define USB_SERIAL_BAUDRATE            115200
//#endif
#define USB_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USB_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USB_SERIAL_STOP_BIT            false  // false = 1, true = 2
#define USB_SERIAL_RX_INT              USARTC0_RXC_vect
#define USB_SERIAL_TX_INT              USARTC0_TXC_vect
#define USB_SERIAL_DRE_INT             USARTC0_DRE_vect

#define USB_SERIAL_RX_BUFFER_SIZE 64
#define USB_SERIAL_TX_BUFFER_SIZE 32

// Initializes the uart for communicating to the computer.
void setupUsb(void);

// Sends a single character over the USB serial.
void sendUSBChar(unsigned char str);

unsigned char recvUSBChar(void);

// Sends a null terminated string over the USB serial. Optionally with the terminator.
void sendUSBString(const char *string, bool terminate);

// Sends a null terminated string over the USB serial and appends a newline.
void sendUSBLine(const char *string);

// Holds or releases the FTDI chip from reset.
void FTDIHoldInReset(bool enable);

// Receives a string, if available.  Returns true if there is a string
// and it can fit in dst; false otherwise.
bool recvUSBString(char* dst, uint8_t max_count);

bool USBPluggedIn(void);

//#if DEBUG
void PrintXY(int16_t x, int16_t y);
//#endif

#endif /* USB_H_ */