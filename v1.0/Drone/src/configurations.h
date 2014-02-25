/*
 * configurations.h
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

#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_

#include "HID.h"
#include "nvm.h"
#include "buttons.h"
#include "joystick_types.h"


enum BT_MODES {
	BTM_KEYBOARD = 0,
	BTM_IOS_KEYBOARD,
	BTM_GAMEPAD,
	BTM_MOUSE,
	BTM_KEYBOARD_MOUSE,
	BTM_JOYSTICK_DIGITAL,
	BTM_SERIAL,
	BTM_JOYSTICK_ANALOG,
	BTM_SIZE
};

struct GlobalHeader {
	uint8_t sleep_delay;   // How long to hold the "Start" button before powering down. (In tenths of seconds).
	uint8_t red_percent;   // The intensity of the red LED (0-100)
	uint8_t blue_percent;  // The intensity of the blue LED (0-100)
	uint8_t green_percent; // The intensity of the green LED (0-100)
	char bluetooth_mode_names[1][1]; // The names the drone will enumerate as through bluetooth. (See enum BT_MODES)
	uint8_t default_configuration :4;
	uint8_t first_run :1;  // 1 - first run, 0 - not first run.
	uint8_t blueSwapGreen :2; // 0 - PORTD, 0=Green and PORTD,1=Blue, 1 - PORTD,0=Blue and PORTD,1=Green
	uint8_t powerOffSoftware :7; // Default Use uC Sleep commands and an Interrupt to wake it up
								// Backup Plan use the components of Page 2 in Schematic for a full power off.
	uint8_t led_G_D0_B_D1 :8; // Default = 0/false behavior is Green on D0 and Blue on D1, 
};

enum ButtonType {
	BUTTON_TYPE_ANALOG  = 0,
	BUTTON_TYPE_DIGITAL = 1
};	

enum InputType {
	INPUT_TYPE_KEYBOARD = 0,
	INPUT_TYPE_MOUSE    = 1
};

enum MouseButton {
	MOUSE_BUTTON_NONE   = 0,
	MOUSE_BUTTON_LEFT   = 1,
	MOUSE_BUTTON_RIGHT  = 2,
	MOUSE_BUTTON_MIDDLE = 4
};

enum ReportType {
	REPORT_RAW = 0,
	REPORT_CONSUMER = 1
};

struct ButtonConfiguration {
	struct {
		uint8_t repeat_rate :6;
		uint8_t button_type :1; // See enum ButtonType
	} config;
	
	struct {
		uint8_t mouse_buttons :3; // See enum MouseButton
		uint8_t input_type    :1; // See enum InputType
		uint8_t report_type   :1; // See enum ReportType
	} keyboard_and_mouse;
	
	struct HIDReport button_up;
	struct HIDReport button_down;
	
	uint8_t analog_min;
	uint16_t analog_max;
};

typedef struct {
	struct {
		uint8_t repeat_rate :6;
		uint8_t button_type :1; // See enum ButtonType
	} config;
	
	struct {
		uint8_t mouse_buttons :3; // See enum MouseButton
		uint8_t input_type    :1; // See enum InputType
		uint8_t report_type   :1; // See enum ReportType
	} keyboard_and_mouse;
	
	struct HIDReport button_up;
	struct HIDReport button_down;
	struct HIDReport button_left;
	struct HIDReport button_right;
	
	int16_t x_min;
	int16_t x_max;
	int16_t y_min;
	int16_t y_max;
	int16_t x_deadzone;
	int16_t y_deadzone;
} JoystickConfiguration;

struct ControllerConfiguration {
	uint8_t mode; // See enum BT_MODES.  This field determines how to configure the controller on boot. 

	struct ButtonConfiguration button_configs[NUM_BUTTONS]; // See enum ButtonName for the index of this array.
	JoystickConfiguration joystick_configs[NUM_JOYSTICKS]; // See enum JoystickName for the index of this array.
};

void readGlobalHeader(struct GlobalHeader *gh);
void writeGlobalHeader(const struct GlobalHeader *gh);
void readControllerConfig(uint8_t index, struct ControllerConfiguration *cc);
void writeControllerConfig(uint8_t index, const struct ControllerConfiguration *cc);

typedef enum {
	CFG_A=0,
	CFG_B,
	CFG_X,
	CFG_Y,
	CFG_RB,
	CFG_DOWN,
	CFG_RIGHT,
	CFG_UP,
	CFG_LEFT,
	CFG_LB,
	CFG_SELECT
} cfgsel_t;

void loadConfiguration(cfgsel_t which_configuration);  // Loads the controller configuration.
cfgsel_t whichConfiguration(void); // which config button pressed?
void displayWhichConfiguration(cfgsel_t configuration);
#endif /* CONFIGURATIONS_H_ */