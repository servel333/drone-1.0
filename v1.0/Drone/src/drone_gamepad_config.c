/*
 * drone_gamepad_config.c
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

#include "drone_game_configurations.h"

#include "analog.h"
//5% deadzones based on ADC range
#define DRONE_X_DEADZONE (DRONE_ADC_MAX-DRONE_ADC_MIN)/50
#define DRONE_Y_DEADZONE (DRONE_ADC_MAX-DRONE_ADC_MIN)/50

const struct ControllerConfiguration DRONE_GAMEPAD_CONFIG = {
	.mode = BTM_GAMEPAD,
	.joystick_configs = {
		{ // LEFT JOYSTICK
			.x_min = -1500,
			.x_max = 1500,
			.y_min = -1500,
			.y_max = 1500,
			.x_deadzone = 300,
			.y_deadzone = 300
		},
		{ // RIGHT JOYSTICK
			.x_min = -1500,
			.x_max = 1500,
			.y_min = -1500,
			.y_max = 1500,
			.x_deadzone = 300,
			.y_deadzone = 300
		}
	}
};