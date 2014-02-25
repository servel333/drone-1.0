/*
 * drone_game_configurations.h
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

#ifndef DRONE_GAME_CONFIGURATIONS_H_
#define DRONE_GAME_CONFIGURATIONS_H_

#include "configurations.h"

extern const struct GlobalHeader DEFAULT_HEADER;

// Built in configurations
extern const struct ControllerConfiguration DRONE_GAMEPAD_CONFIG;			// Button A
extern const struct ControllerConfiguration DRONE_IOS_CONFIG;				// Button B
extern const struct ControllerConfiguration DRONE_KEYBOARD_CONFIG;			// Button X
extern const struct ControllerConfiguration DRONE_KEYBOARD_MOUSE_CONFIG;	// Button Y
extern const struct ControllerConfiguration DRONE_JOYSTICK_CONFIG;			// Button RB

// Open configurations for custom configurations
//extern const struct ControllerConfiguration DRONE_OPEN_CONFIG_1;	// Button DPAD DOWN
//extern const struct ControllerConfiguration DRONE_OPEN_CONFIG_2;	// Button DPAD RIGHT
//extern const struct ControllerConfiguration DRONE_OPEN_CONFIG_3;	// Button DPAD LEFT
//extern const struct ControllerConfiguration DRONE_OPEN_CONFIG_4;	// Button DPAD UP
//extern const struct ControllerConfiguration DRONE_OPEN_CONFIG_5;	// Button LB

#endif /* DEFAULT_CONFIGURATIONS_H_ */