/*
 * debug.c 
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

#include "debug.h"
#include "asf.h"
#include <avr\pgmspace.h>
#include "strings.h"
#include "watchdog.h"

#define DBG_SERIAL                     &USARTD0
#define DBG_SERIAL_BAUDRATE            115200
#define DBG_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define DBG_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define DBG_SERIAL_STOP_BIT            false  // false = 1, true = 2

void setupDbg(void)
{
	//sysclk_init();
	const usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = DBG_SERIAL_BAUDRATE,
		.charlength = DBG_SERIAL_CHAR_LENGTH,
		.paritytype = DBG_SERIAL_PARITY,
		.stopbits = DBG_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	sendDBGString( (char *) pgm_read_byte(debuguartinit));
	WatchdogPet();
	if (usart_init_rs232(DBG_SERIAL, &USART_SERIAL_OPTIONS)) {
		//sendDBGLine(pgm_read_byte(good));
		sendDBGLine( (char *) pgm_read_byte(good));
	}
	else
	{		
		//sendDBGLine(pgm_read_byte(bad));
		sendDBGLine( (char *) pgm_read_byte(bad));
	}
}

void sendDBGChar(unsigned char str)
{
	usart_putchar(DBG_SERIAL, str);
};

unsigned char recvDBGChar(void)
{
	return usart_getchar(DBG_SERIAL);
};

// write out a simple '\0' terminated string
void sendDBGString(const char *string)
{
	while(*string != 0)
	sendDBGChar(*string++);
};

// write out a simple '\0' terminated string and print "\n\r" at end
void sendDBGLine(const char *string)
{
	sendDBGString(string);
	sendDBGChar('\r');
	sendDBGChar('\n');
};