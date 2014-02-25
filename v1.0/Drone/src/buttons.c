/*
 * buttons.c
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

#include "buttons.h"

/*
  Defines how each button is configured and holds the state.
  This must follow in the same order as enum ButtonNames.
*/
struct Button buttons[NUM_BUTTONS] = {
{ UP_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ DOWN_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ LEFT_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ RIGHT_PIN,		0, BUTTON_RELEASED, ACTIVE_LOW},
{ A_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ B_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ X_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ Y_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ START_PIN,		0, BUTTON_RELEASED, ACTIVE_HIGH},
{ SELECT_PIN,		0, BUTTON_RELEASED, ACTIVE_LOW},
{ RB_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ LB_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ RT_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ LT_PIN,			0, BUTTON_RELEASED, ACTIVE_LOW},
{ JOYSTICK_L_CLICK, 0, BUTTON_RELEASED, ACTIVE_LOW},
{ JOYSTICK_R_CLICK, 0, BUTTON_RELEASED, ACTIVE_LOW}
};

bool buttonActivated(enum ButtonName bn) {
	if(ioport_get_pin_level(buttons[bn].pin)) {
		return buttons[bn].polarity;
	}
	else {
		return !buttons[bn].polarity;
	}	
}