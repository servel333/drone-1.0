/*
 * gamepad.h
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

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "buttons.h"
#include "joysticks.h"

#define GAMEPAD_RESOLUTION 1

// These are defined in the joystick HID descriptor:
#define GAMEPAD_X_MIN -127
#define GAMEPAD_X_MAX  127
#define GAMEPAD_Y_MIN -127
#define GAMEPAD_Y_MAX  127

void gamepadInit(struct ControllerConfiguration *cc);
void gamepadButtonHandler(enum ButtonName name, enum ButtonState state);
void gamepadJoystickHandler(enum JoystickName name, int16_t x, int16_t y);


#endif /* GAMEPAD_H_ */