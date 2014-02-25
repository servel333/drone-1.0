/*
 * default_configurations.c
 * Version: Drone 1.0
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

#include "default_configurations.h"

const struct GlobalHeader DEFAULT_HEADER = {
	.sleep_delay = 50,
	.red_percent = 1,
	.green_percent = 1,
	.blue_percent = 1,
	.bluetooth_mode_names = {
	  "Drone Keyboard",
	  "Drone iOS",
	  "Drone GamePad",
	  "Drone Key Mouse",
	  "Drone Serial",
	  "Drone Joystick"
	},
 	.default_configuration = 0,
	.first_run = 1,
	.powerOffSoftware = 1,
	.led_G_D0_B_D1 = 0
};