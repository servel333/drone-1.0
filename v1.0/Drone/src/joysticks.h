/*
 * joysticks.h
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

#ifndef JOYSTICKS_H_
#define JOYSTICKS_H_

#include <asf.h>

#include "shared.h"
#include "configurations.h"
#include "joystick_types.h"

#define GAMEPAD_X_MIN -127
#define GAMEPAD_X_MAX  127
#define GAMEPAD_Y_MIN -127
#define GAMEPAD_Y_MAX  127

#define MOUSE_X_MIN -2048
#define MOUSE_X_MAX  2047
#define MOUSE_Y_MIN -2048
#define MOUSE_Y_MAX  2047



extern struct Joystick joysticks[NUM_JOYSTICKS];

//Center calibration, Unsigned to Signed conversion, and axis flipping
//compare AnalogJoystickAdj
void SimpleJoystickAdj( const uint16_xy * center, const int16_xy * deadzone, const uint16_xy * raw, int16_xy * out );

void AnalogJoystickAdj( const uint16_xy * center, JoystickConfiguration *this_joystick_cfg, const uint16_xy * raw, int8_xy * out );

void MouseJoystickAdj( const uint16_xy * center, JoystickConfiguration *this_joystick_cfg, const uint16_xy * raw, int16_xy * out );

#endif /* JOYSTICKS_H_ */