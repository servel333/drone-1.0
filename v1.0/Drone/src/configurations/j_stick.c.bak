/*
 * j_stick.c
 * Version: Drone Open Beta 1.0
 *
 * Copyright (C) 2013 EVO CONTROLLERS, LLC.
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
#include "j_stick.h"
#include "debug.h"
#include "bluetooth.h"
#include "configurations.h"
#include "system_time.h"

// Local structure for holding the joystick positions.
struct JStickJoystickXY {
	int8_t x;
	int8_t y;
};

static struct JStickJoystickXY current_xy[NUM_JOYSTICKS];
static struct JStickJoystickXY previous_xy[NUM_JOYSTICKS];

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

// Local for holding the currently pressed buttons.
static uint32_t pressed_buttons = 0;
static uint32_t previous_pressed_buttons = 0;

// Local function to handle keyboard and joystick button presses.
void jStickReport(void);

void jStickInit(struct ControllerConfiguration *cc) {
	my_cc = cc;
	sendDBGLine("jStickInit");
	BluetoothJoystickMode();
}

void jStickButtonHandler(enum ButtonName name, enum ButtonState state) {
	if (state == BUTTON_PRESSED) {
		// Set the corresponding bit:
		pressed_buttons |= 1ul<<name;
	}

	if (state == BUTTON_RELEASED) {
		// Clear the corresponding bit:
		pressed_buttons &= ~(1ul<<name);
	}
	
	jStickReport();
}

void jStickJoystickHandler(enum JoystickName name, int16_t x, int16_t y) {
	struct Joystick *this_joystick = &joysticks[name];
	struct JoystickConfiguration *this_joystick_cfg = &my_cc->joystick_configs[name];

	// Correct for the center position:
	x -= this_joystick->center_x;
	y -= this_joystick->center_y;
	
	// Check if the magnitude is greater than the deadzone:
	if (x > this_joystick_cfg->x_deadzone) {
		x -= this_joystick_cfg->x_deadzone;
	}
	else if (x < -this_joystick_cfg->x_deadzone) {
		x += this_joystick_cfg->x_deadzone;
	}
	else {
		x = 0;
	}
	if (y > this_joystick_cfg->y_deadzone) {
		y -= this_joystick_cfg->y_deadzone;
	}
	else if (y < -this_joystick_cfg->y_deadzone) {
		y += this_joystick_cfg->y_deadzone;
	}
	else {
		y = 0;
	}
	
	// Adjust for sensitivity:
	x /= JSTICK_RESOLUTION;
	y /= JSTICK_RESOLUTION;
	
	// Min and max are defined as -127 to 127.  Fix if out of bounds:
	if (x > JSTICK_X_MAX) {
		x = JSTICK_X_MAX;
	}
	else if (x < JSTICK_X_MIN) {
		x = JSTICK_X_MIN;
	}
	if (y > JSTICK_Y_MAX) {
		y = JSTICK_Y_MAX;
	}
	else if (y < JSTICK_Y_MIN) {
		y = JSTICK_Y_MIN;
	}

	// Record the position:
	current_xy[name].x = (int8_t)x;
	current_xy[name].y = (int8_t)y;
	
	jStickReport();
}

// Sends a jStick report when needed:
void jStickReport(void) {

	// Send a report if the joystick or buttons have changed:
	if (pressed_buttons != previous_pressed_buttons
	    || current_xy[JOYSTICK_LEFT].x != previous_xy[JOYSTICK_LEFT].x
		|| current_xy[JOYSTICK_LEFT].y != previous_xy[JOYSTICK_LEFT].y
		|| current_xy[JOYSTICK_RIGHT].x != previous_xy[JOYSTICK_RIGHT].x
		|| current_xy[JOYSTICK_RIGHT].y != previous_xy[JOYSTICK_RIGHT].y) {
		
		// Update the previous values:
		memcpy(&previous_xy, &current_xy, sizeof(previous_xy));
		previous_pressed_buttons = pressed_buttons;

#if DEBUG == 1
		int8_t i;
		int8_t temp1, temp2;
		for (i = 0; i < NUM_BUTTONS; i++) {
			sendDBGChar((pressed_buttons & (1ul<<i))? '1':'0');
		}
		
		temp1 = current_xy[JOYSTICK_LEFT].x;
		sendDBGString(", X1=");
		if (temp1 < 0) {
			sendDBGChar('-');
			temp1 = -temp1;
		}
		for (i = 100; i != 0; i /= 10) {
			sendDBGChar('0' + (temp1/i));
			temp1 %= i;
		}
		temp1 = current_xy[JOYSTICK_LEFT].y;
		sendDBGString(", Y1=");
		if (temp1 < 0) {
			sendDBGChar('-');
			temp1 = -temp1;
		}
		for (i = 100; i != 0; i /= 10) {
			sendDBGChar('0' + (temp1/i));
			temp1 %= i;
		}
		temp1 = current_xy[JOYSTICK_RIGHT].x;
		sendDBGString(", X2=");
		if (temp1 < 0) {
			sendDBGChar('-');
			temp1 = -temp1;
		}
		for (i = 100; i != 0; i /= 10) {
			sendDBGChar('0' + (temp1/i));
			temp1 %= i;
		}
		temp1 = current_xy[JOYSTICK_RIGHT].y;
		sendDBGString(", Y2=");
		if (temp1 < 0) {
			sendDBGChar('-');
			temp1 = -temp1;
		}
		for (i = 100; i != 0; i /= 10) {
			sendDBGChar('0' + (temp1/i));
			temp1 %= i;
		}
		sendDBGLine("");
#endif


#if (BT_BINARY_MODE == 1)
		// Send the joystick report:
		// Note there is no separate joystick report for the Nova Bluetooth module
		// So send the gamepad report.
		/*
			* The format of the gamepad report:
			*
			* Identifier (A1,04)
			* Throttle
			* X1
			* Y1
			* X2
			* Y2
			* Buttons 7-0
			* Buttons 15-8
			* HatSwitch
			*/
		// Send the identifier/size/blank-byte:
		sendBTChar('\xA1');
		sendBTChar('\x04');
		// Send the throttle (not used)
		sendBTChar(0);
		// Send the left joystick values:
		sendBTChar(current_xy[JOYSTICK_LEFT].x);
		sendBTChar(current_xy[JOYSTICK_LEFT].y);
		
		// Send the right joystick values:
		sendBTChar(current_xy[JOYSTICK_RIGHT].x);
		sendBTChar(current_xy[JOYSTICK_RIGHT].y);

		// Send the hat switch (no movement)
		sendBTChar(8);
		// Send the button states:
		sendBTChar((uint8_t)(pressed_buttons & 0x00FF));
		sendBTChar((uint8_t)((pressed_buttons >> 8) & 0x00FF));

#else
		// Send the joystick report:
		// Note there is no separate joystick report for the Nova Bluetooth module
		// So send the gamepad report.
		/*
			* The format of the gamepad report:
			*
			* Identifier (A1,04)
			* Throttle
			* X1
			* Y1
			* X2
			* Y2
			* Buttons 7-0
			* Buttons 15-8
			* HatSwitch
			*/
		// Send the identifier/size/blank-byte:
		sendBTString("BC:KR=A1,04,");
		// Send the throttle (not used)
		sendBTHexPair(0);
		// Send the left joystick values:
		sendBTChar(',');
		sendBTHexPair(current_xy[JOYSTICK_LEFT].x);
		sendBTChar(',');
		sendBTHexPair(current_xy[JOYSTICK_LEFT].y);
		
		// Send the right joystick values:
		sendBTChar(',');
		sendBTHexPair(current_xy[JOYSTICK_RIGHT].x);
		sendBTChar(',');
		sendBTHexPair(current_xy[JOYSTICK_RIGHT].y);

		// Send the button states:
		sendBTChar(',');
		sendBTHexPair((uint8_t)(pressed_buttons & 0x00FF));
		sendBTChar(',');
		sendBTHexPair((uint8_t)((pressed_buttons >> 8) & 0x00FF));
		// Send the hat switch (no movement)
		sendBTChar(',');
		sendBTHexPair(8);
		sendBTChar('\r');
		sendBTChar('\n');
#endif		
	}
}