/*
 * buttons.h
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

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <asf.h>

#define DEBOUNCE_PERIOD 4 // The number of samples needed to cause a button to change states.

// Forward declaration of controller config:
struct ControllerConfiguration;

// This enum is used to identify the buttons at all levels
enum ButtonName {
	B_UP = 0,
	B_DOWN,
	B_LEFT,
	B_RIGHT,
	B_A,
	B_B,
	B_X,
	B_Y,
	B_START,
	B_SELECT,
	B_RB,
	B_LB,
	B_RT,
	B_LT,
	B_JL,
	B_JR,
	NUM_BUTTONS,
	REPORT
};

// The current state of a button.
enum ButtonState {
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_LONG_PRESS
};

// The polarity of a button.
enum Polarity {
	ACTIVE_LOW = 0,
	ACTIVE_HIGH 
};

// Used to handle button detection.
struct Button {
	ioport_pin_t     pin;              // The I/O pin to check for a button press.
	uint8_t          debounce_counter; // Counts samples and looks for pin change state.
	enum ButtonState state;            // The current state of the button.
	enum Polarity    polarity;         // Whether the button is active low or active high.
};

// Function pointer type to issue button presses and releases.
typedef void (*ButtonHandler)(enum ButtonName, enum ButtonState);

// Function pointer to initialize the button handler.  At this point the
// bluetooth module will already be in command mode.  This function should
// not exit command mode either.
typedef void (*ButtonHandlerInit)(struct ControllerConfiguration *cc);

// Holds the data structure for each button.  The index of this array is enum ButtonName.
extern struct Button buttons[NUM_BUTTONS];

bool buttonActivated(enum ButtonName bn);



#endif /* BUTTONS_H_ */