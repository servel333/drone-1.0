/*
 * keyboard_ios.c
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

#include <asf.h>
#include "keyboard_ios.h"
#include "debug.h"
#include "bluetooth.h"
#include "configurations.h"

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

void keyboardiOSInit(struct ControllerConfiguration *cc) {
	my_cc = cc;
    BluetoothKeyboardMode();
}

void keyboardiOSButtonHandler(enum ButtonName name, enum ButtonState state) {
	char key;
	unsigned char i;
	
	// Get the ASCII character to press:
	if (state == BUTTON_PRESSED) {
		key = my_cc->button_configs[name].button_down.scan_code;
	}
	else {
		key = my_cc->button_configs[name].button_up.scan_code;
	}
	
	if (key != 0) {
		sendDBGChar(key);
		
		#if (BT_BINARY_MODE == 1)
		// Send the identifier:
		sendBTChar('\xA1');
		sendBTChar('\x01');
		sendBTChar('\x00');

		// Send the modifiers:
		sendBTChar(0);
		
		// send the scan code as the first code
		sendBTChar(key);
		
		// Pad the other scan codes with 0
		for (i=0;i < 5; i++) {
			sendBTChar(0);
		}
		
		// Now send the no-scan-code so the button released is recognized
		// Send the identifier:
		sendBTChar('\xA1');
		sendBTChar('\x01');
		sendBTChar('\x00');
		
		// Send the modifiers:
		sendBTChar(0);
		
		// Pad the scan codes with 0s
		for (i=0;i < 6; i++) {
			sendBTChar(0);
		}
		
		#else
		// Send the identifier:
		sendBTString("BC:KR=A1,01,00,");
		
		// Send the modifiers:
		sendBTHexPair(0);
		
		// send the scan code as the first code
		sendBTChar(',');
		sendBTHexPair(key);
		
		// Pad the other scan codes with 0
		for (i=0;i < 5; i++) {
			sendBTChar(',');
			sendBTHexPair(0);
		}
		sendBTChar('\r'); 
		sendBTChar('\n');
		
		// Now send the no-scan-code so the button released is recognized
		// Send the identifier:
		sendBTString("BC:KR=A1,01,00,");
		
		// Send the modifiers:
		sendBTHexPair(0);
		
		// Pad the scan codes with 0s
		for (i=0;i < 6; i++) {
			sendBTChar(',');
			sendBTHexPair(0);
		}
		sendBTChar('\r'); 
		sendBTChar('\n');
		#endif
	}
};

	
	