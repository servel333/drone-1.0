/**
 * keyboard_mouse.c
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
#include "keyboard_mouse.h"
#include "debug.h"
#include "bluetooth.h"
#include "configurations.h"
#include "system_time.h"

#include "usb.h"

const static uint16_xy * center[2] = {(uint16_xy*)&(joysticks[JOYSTICK_LEFT].center_x),(uint16_xy*)&(joysticks[JOYSTICK_RIGHT].center_x)};

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

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

// Local for holding the currently pressed buttons and/or joystick buttons.
static uint32_t pressed_buttons = 0;

// Local for holding mouse button states.
static uint8_t mouse_buttons = 0;

// Local for determining when to send a mouse hid report.
static struct Timeout mouse_timeout;

// Local function to handle keyboard and joystick button presses.
void changeButtonState(uint8_t name, enum ButtonState state);

// External reference to system time
extern uint32_t getSystemTime(void);

void keyboardMouseInit(struct ControllerConfiguration *cc) {
	my_cc = cc;
	BluetoothComboMode();
	startTimeout(&mouse_timeout, MOUSE_REPORT_INTERVAL_MS);
}

void keyboadMouseButtonHandler(enum ButtonName name, enum ButtonState state) {
	changeButtonState((uint8_t)name, state);
}

void keyboardMouseJoystickHandler(enum JoystickName name, uint16_t x, uint16_t y) {
	static uint8_t last_mouse_buttons = 0;
	static enum JoystickButtonState state_x[NUM_JOYSTICKS];
	static enum JoystickButtonState state_y[NUM_JOYSTICKS];
	JoystickConfiguration *this_joystick;
	//static int8_t mouse_x = 0, mouse_y = 0;
	int16_xy mouse = {0,0};
	uint8_t joystick_button_base;
	//uint16_t x_center, y_center;
	uint8_t mouseTmp;
	
	uint16_xy raw = {x,y};
	
	// Get information about this joystick:
	this_joystick = &(my_cc->joystick_configs[name]);

	// Get the base index for these joystick buttons:	
	joystick_button_base = NUM_BUTTONS + 4 * name;


	// Find the center locations of the joysticks:
	//x_center = joysticks[name].center_x;
	//y_center = joysticks[name].center_y;
	
	if (this_joystick->config.button_type == BUTTON_TYPE_ANALOG) 
	{
		// Is this joystick analog?
		
		//TODO: add scaling (by MOUSE_RESOLUTION) in AnalogJoystickAdj - pre-working gamepad joysticks?
		MouseJoystickAdj( center[name], this_joystick, &raw, &mouse );	
		
		//#if DEBUG
		//	//Write raw and adjusted X,Y values to USB serial port
		//	PrintXY(raw.x,raw.y);
		//	sendUSBString("->", false);
		//	PrintXY(mouse.x,mouse.y);
		//	sendUSBLine("");	
		//#endif
		
		//mouse.x /= MOUSE_RESOLUTION;
		//mouse.y /= MOUSE_RESOLUTION;
		
        if(mouse.x)
		{
			mouse.x /= MOUSE_RESOLUTION;
		}
		
		if(mouse.y)
		{
			mouse.y /= MOUSE_RESOLUTION;
		}
		
		
	}
	else 
	{
		// Otherwise it must be a digital joystick.
		
		//int16_xy deadzone = {this_joystick->x_deadzone,this_joystick->y_deadzone};
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
					changeButtonState(joystick_button_base + JBSTATE_LEFT, BUTTON_PRESSED);
				}
				// Do we need to enter the right state?
				else if( out.x>0 )
				{
					state_x[name] = JBSTATE_RIGHT;
					changeButtonState(joystick_button_base + JBSTATE_RIGHT, BUTTON_PRESSED);
				}
				break;
			case JBSTATE_LEFT:
				// Do we need to leave the left state?
				//if (x > this_joystick->x_min) {
				if( out.x>=0 )
				{
					state_x[name] = JBSTATE_IDLE;
					changeButtonState(joystick_button_base + JBSTATE_LEFT, BUTTON_RELEASED);
				}
				break;
			case JBSTATE_RIGHT:
				// Do we need to leave the right state?
				if( out.x<=0 )
				{
					state_x[name] = JBSTATE_IDLE;
					changeButtonState(joystick_button_base + JBSTATE_RIGHT, BUTTON_RELEASED);
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
					changeButtonState(joystick_button_base + JBSTATE_UP, BUTTON_PRESSED);
				}
				// Do we need to enter the down state?
				else if( out.y>0 )
				{
					state_y[name] = JBSTATE_DOWN;
					changeButtonState(joystick_button_base + JBSTATE_DOWN, BUTTON_PRESSED);
				}
				break;
			case JBSTATE_UP:
				// Do we need to leave the up state?
				if( out.y>=0 )
				{
					state_y[name] = JBSTATE_IDLE;
					changeButtonState(joystick_button_base + JBSTATE_UP, BUTTON_RELEASED);
				}
				break;
			case JBSTATE_DOWN:
				// Do we need to leave the down state?
				if( out.y<=0 )
				{
					state_y[name] = JBSTATE_IDLE;
					changeButtonState(joystick_button_base + JBSTATE_DOWN, BUTTON_RELEASED);
				}
				break;
			default:
				state_y[name] = JBSTATE_IDLE;
				break;
		};
	}

	// Do we need to notify the host of a mouse change?
	if (mouse.x != 0 || mouse.y != 0 || mouse_buttons != last_mouse_buttons) {
		
		#if DEBUG
			//Write raw and adjusted X,Y values to USB serial port
			PrintXY(raw.x,raw.y);
			sendUSBString("->", false);
			PrintXY(mouse.x,mouse.y);
			sendUSBLine("");
		#endif	
		
		// Is it time to send a mouse report?
		if (checkTimeout(&mouse_timeout)) {
			// Restart the timeout:
			startTimeout(&mouse_timeout, MOUSE_REPORT_INTERVAL_MS);
			
			// Record the mouse button presses:
			last_mouse_buttons = mouse_buttons;
#if (BT_BINARY_MODE == 1)
			// Send the report. The format is
			// A1,03,buttons,XmXl,YlXh,YhYm,wheel
			sendBTChar(0xA1);
			sendBTChar(0x03);
			sendBTChar(mouse_buttons);
			mouseTmp = mouse.x & 0xFF;
			sendBTChar(mouseTmp);
			mouseTmp = ((mouse.x >> 8) & 0x0F) | ((mouse.y & 0x0F) << 4);
			sendBTChar(mouseTmp);
			mouseTmp = (mouse.y >> 4) & 0xFF;
			sendBTChar(mouseTmp);
			sendBTChar(0x00); // Wheel position; unused
#else			
			// Send the report. The format is
			// A1,03,buttons,XmXl,YlXh,YhYm,wheel
			sendBTString("BC:KR=A1,03,");
			sendBTHexPair(mouse_buttons);
			sendBTChar(',');			
			mouseTmp = mouse.x & 0xFF;
			sendBTHexPair(mouseTmp);
			sendBTChar(',');
			mouseTmp = ((mouse.x >> 8) & 0x0F) | ((mouse.y & 0x0F) << 4);
			sendBTHexPair(mouseTmp);
			sendBTChar(',');
			mouseTmp = (mouse.y >> 4) & 0xFF;
			sendBTHexPair(mouseTmp);
			sendBTChar(',');
			sendBTHexPair(0x00); // Wheel position; unused
			sendBTChar('\r');
			sendBTChar('\n');
#endif
			// Reset mouse x and y:
			mouse.x = 0;
			mouse.y = 0;
		}		
	}
}

// Similar to keyboadMouseButtonHandler, but will handle
// both "enum ButtonName" and "enum JoystickButtonName".
void changeButtonState(uint8_t name, enum ButtonState state) {
	uint8_t scan_codes[MAX_SCANCODES]; 
	uint8_t modifiers = 0;
	uint8_t count;
	struct ButtonConfiguration *this_button;
	struct HIDReport *this_joystick_button;
	uint8_t i;
	
	// Clear the mouse buttons:
	mouse_buttons = 0;
	
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
			// Otherwise this must be a mouse button.
			else {
				// OR any mouse buttons together:
				mouse_buttons |= this_button->keyboard_and_mouse.mouse_buttons;
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

#if (BT_BINARY_MODE == 1)
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
	
#else
	sendBTString("BC:KR=A1,01,00,");
	// Send the modifiers:
	sendBTHexPair(modifiers);
	// send the 6 scan codes
	for (i=0;i < 6; i++) {
		sendBTChar(',');
		if (i < count) {
			sendBTHexPair(scan_codes[i]);
		}
		// Otherwise send the no-keys-pressed event:
		else {
			// Send the no-keys-pressed event:
			sendBTHexPair(0);
		}
	
	}
	sendBTChar('\r');
	sendBTChar('\n');
#endif	
}

