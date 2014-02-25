/*
* keyboard.c
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
#include <string.h>
#include "keyboard.h"
#include "debug.h"
#include "bluetooth.h"
#include "configurations.h"

// The maximum number of scan codes that can be transmitted in a HID report.
#define MAX_SCANCODES 6

// These are used to handle a digital joystick.
enum JoystickButtonName {
	B_JOYSTICK_LEFT_UP = NUM_BUTTONS,
	B_JOYSTICK_LEFT_DOWN,
	B_JOYSTICK_LEFT_LEFT,
	B_JOYSTICK_LEFT_RIGHT,
	B_JOYSTICK_RIGHT_UP,
	B_JOYSTICK_RIGHT_DOWN,
	B_JOYSTICK_RIGHT_LEFT,
	B_JOYSTICK_RIGHT_RIGHT,
	NUM_ALL_BUTTONS
};

// Must match order of enum JoystickButtonName.
enum JoystickButtonState {
	JBSTATE_UP,
	JBSTATE_DOWN,
	JBSTATE_LEFT,
	JBSTATE_RIGHT,
	JBSTATE_IDLE
};

const static uint16_xy * center[2] = {(uint16_xy*)&(joysticks[JOYSTICK_LEFT].center_x),(uint16_xy*)&(joysticks[JOYSTICK_RIGHT].center_x)};

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

// Local for holding the currently pressed buttons and/or joystick buttons.
static uint32_t pressed_buttons = 0;

// Local function to handle keyboard button presses.
void ChangeButtonPressState(uint8_t name, enum ButtonState state);

void keyboardInit(struct ControllerConfiguration *cc) {
	my_cc = cc;
	BluetoothKeyboardMode();
}

void keyboardButtonHandler(enum ButtonName name, enum ButtonState state) {
	ChangeButtonPressState((uint8_t)name, state);
}

void keyboardJoystickHandler(enum JoystickName name, uint16_t x, uint16_t y) {
	static enum JoystickButtonState state_x[NUM_JOYSTICKS];
	static enum JoystickButtonState state_y[NUM_JOYSTICKS];
	JoystickConfiguration *this_joystick;
	uint8_t joystick_button_base;

	// Get information about this joystick:
	this_joystick = &(my_cc->joystick_configs[name]);

	// Get the base index for these joystick buttons:
	joystick_button_base = NUM_BUTTONS + 4 * name;
	
	uint16_xy raw = {x,y};
		
		int16_xy * deadzone = (int16_xy *) &(this_joystick->x_deadzone);
		int16_xy out;
		SimpleJoystickAdj( center[name], deadzone, &raw, &out );
		
		// Check the X axis:
		switch (state_x[name]) {
			case JBSTATE_IDLE:
			// Do we need to enter the left state?
			if( out.x<0 )
			{
				state_x[name] = JBSTATE_LEFT;
				ChangeButtonPressState(joystick_button_base + JBSTATE_LEFT, BUTTON_PRESSED);
			}
			// Do we need to enter the right state?
			else if( out.x>0 )
			{
				state_x[name] = JBSTATE_RIGHT;
				ChangeButtonPressState(joystick_button_base + JBSTATE_RIGHT, BUTTON_PRESSED);
			}
			break;
			case JBSTATE_LEFT:
			// Do we need to leave the left state?
			if( out.x>=0 )
			{
				state_x[name] = JBSTATE_IDLE;
				ChangeButtonPressState(joystick_button_base + JBSTATE_LEFT, BUTTON_RELEASED);
			}
			break;
			case JBSTATE_RIGHT:
			// Do we need to leave the right state?
			if( out.x<=0 )
			{
				state_x[name] = JBSTATE_IDLE;
				ChangeButtonPressState(joystick_button_base + JBSTATE_RIGHT, BUTTON_RELEASED);
			}
			break;
			default:
			state_x[name] = JBSTATE_IDLE;
			break;
		};

		// Check the Y axis:
		switch (state_y[name]) {
			case JBSTATE_IDLE:
			// Do we need to enter the up state?
			if( out.y<0 )
			{
				state_y[name] = JBSTATE_UP;
				ChangeButtonPressState(joystick_button_base + JBSTATE_UP, BUTTON_PRESSED);
			}
			// Do we need to enter the down state?
			else if( out.y>0 )
			{
				state_y[name] = JBSTATE_DOWN;
				ChangeButtonPressState(joystick_button_base + JBSTATE_DOWN, BUTTON_PRESSED);
			}
			break;
			case JBSTATE_UP:
			// Do we need to leave the up state?
			if( out.y>=0 )
			{
				state_y[name] = JBSTATE_IDLE;
				ChangeButtonPressState(joystick_button_base + JBSTATE_UP, BUTTON_RELEASED);
			}
			break;
			case JBSTATE_DOWN:
			// Do we need to leave the down state?
			if( out.y<=0 )
			{
				state_y[name] = JBSTATE_IDLE;
				ChangeButtonPressState(joystick_button_base + JBSTATE_DOWN, BUTTON_RELEASED);
			}
			break;
			default:
			state_y[name] = JBSTATE_IDLE;
			break;
		};	
}

void ChangeButtonPressState(uint8_t name, enum ButtonState state) {
	uint8_t scan_codes[MAX_SCANCODES];
	uint8_t modifiers = 0;
	uint8_t count;
	struct ButtonConfiguration *this_button;
	struct HIDReport *this_joystick_button;
	uint8_t i;
	
	if (state == BUTTON_PRESSED) {
		// Set the corresponding bit:
		pressed_buttons |= 1ul<<name;
	}

	if (state == BUTTON_RELEASED) {
		// Clear the corresponding bit:
		pressed_buttons &= ~(1ul<<name);
	}

	// Find all the pressed buttons (max 6):
	for (i = 0, count = 0; i < NUM_BUTTONS && count < MAX_SCANCODES; i++) {
		// Is this button pressed?
		if (pressed_buttons & (1ul<<i)) {
			// Get information about this button:
			this_button = &(my_cc->button_configs[i]);
			
			// Is it a keyboard button?
			if (this_button->keyboard_and_mouse.input_type == INPUT_TYPE_KEYBOARD) {
				// Does this button have a scancode associated with it?
				if (this_button->button_down.scan_code != 0) {
					// Is this a new scancode for the list?
					if (memchr(&scan_codes, this_button->button_down.scan_code, count) == NULL) {
						// Then add it to the hid report:
						scan_codes[count] = this_button->button_down.scan_code;
						
						// Increment the count:
						count++;
					}
				}
				
				// OR any modifiers for this button:
				modifiers |= this_button->button_down.modifier;
			}
		}
	}
	
	// Find all the "pressed" joystick directions:
	for (i = NUM_BUTTONS; i < NUM_ALL_BUTTONS && count < MAX_SCANCODES; i++) {
		// Is this joystick direction pressed?
		if (pressed_buttons & (1ul<<i)) {
			// Get information about this joystick direction:
			switch (i) {
				case B_JOYSTICK_LEFT_UP:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_LEFT].button_up);
				break;
				case B_JOYSTICK_LEFT_DOWN:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_LEFT].button_down);
				break;
				case B_JOYSTICK_LEFT_LEFT:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_LEFT].button_left);
				break;
				case B_JOYSTICK_LEFT_RIGHT:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_LEFT].button_right);
				break;
				case B_JOYSTICK_RIGHT_UP:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_RIGHT].button_up);
				break;
				case B_JOYSTICK_RIGHT_DOWN:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_RIGHT].button_down);
				break;
				case B_JOYSTICK_RIGHT_LEFT:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_RIGHT].button_left);
				break;
				case B_JOYSTICK_RIGHT_RIGHT:
				this_joystick_button = &(my_cc->joystick_configs[JOYSTICK_RIGHT].button_right);
				break;
				default:
				continue;
			};
			
			// Is there a button press associated with this direction?
			if (this_joystick_button->scan_code != 0) {
				// Is this a new scancode for the list?
				if (memchr(scan_codes, this_joystick_button->scan_code, count) == NULL) {
					// Then add it to the hid report:
					scan_codes[count] = this_joystick_button->scan_code;
					
					// Increment the count:
					count++;
				}
			}
			
			// OR any modifiers for this button:
			modifiers |= this_joystick_button->modifier;
		}
	}
	
	/*
	* The format of the keyboard shorthand report is:
	*
	* Identifier (0xA1, 0x01)
	* Modifiers  (See enum HIDModifier)
	* Zero byte 0
	* Scancode 1
	* Scancode 2
	* Scancode 3
	* Scancode 4
	* Scancode 5
	* Scancode 6
	*/

	// Send the identifier:
	sendBTChar('\xA1');
	sendBTChar('\x01');
	sendBTChar('\x00');
	sendBTChar(modifiers);
	// send the 6 scan codes
	for (i=0;i < 6; i++) {
		if (i < count) {
			sendBTChar(scan_codes[i]);
		}
		// Otherwise send the no-keys-pressed event:
		else {
			// Send the no-keys-pressed event:
			sendBTChar(0);
		}
	}
}