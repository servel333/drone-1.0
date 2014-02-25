/*
 * usb_serial.c
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
#include "usb_serial.h"
#include "debug.h"
#include "usb.h"

void usbSerialInit(struct ControllerConfiguration *cc) {
	// Nothing really to do here.
}

void usbSerialButtonHandler(enum ButtonName name, enum ButtonState state) {
	char key;
	// TODO: grab from the current configuration.
	
	switch (name) {
		case B_A:
		key = 'A';
		break;
		case B_B:
		key = 'B';
		break;
		case B_X:
		key = 'X';
		break;
		case B_Y:
		key = 'Y';
		break;
		default:
		sendDBGLine("Unknown button.");
		break;
	}
	
	if (state == BUTTON_RELEASED) {
		key |= 32; // Shift
	}
	
	sendUSBChar(key);
};
