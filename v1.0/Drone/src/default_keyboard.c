/*
 * default_keyboard.c
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

const struct ControllerConfiguration DEFAULT_KEYBOARD_CONFIG = {
	.mode = BTM_KEYBOARD,
	.button_configs = {
		{ // UP
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'w'-'a'+4
			}
		},
		{ // DOWN
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 's'-'a'+4
			}
		},
		{ // LEFT
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'a'-'a'+4
			}
		},
		{ // RIGHT
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'd'-'a'+4
			}
		},
		{ // A
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'j'-'a'+4
			}
		},
		{ // B
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'k'-'a'+4
			}
		},
		{ // X
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'm'-'a'+4
			}
		},
		{ // Y
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'i'-'a'+4
			}
		},
		{ // START
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'y'-'a'+4
			}
		},
		{ // SELECT
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'r'-'a'+4
			}
		},
		{ // RB
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'p'-'a'+4
			}
		},
		{ // LB
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'q'-'a'+4
			}
		},
		{ // RT
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'p'-'a'+4
			}
		},
		{ // LT
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.keyboard_and_mouse = {
				.input_type = INPUT_TYPE_KEYBOARD
			},
			.button_down = {
				.scan_code = 'q'-'a'+4
			}
		}
	},
	.joystick_configs = {
		{ // LEFT JOYSTICK
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.button_up = {
				.scan_code = 'w'-'a'+4
			},
			.button_down = {
				.scan_code = 's'-'a'+4
			},
			.button_left = {
				.scan_code = 'a'-'a'+4
			},
			.button_right = {
				.scan_code = 'd'-'a'+4
			},
			.x_min = 2000,
			.x_max = 3500,
			.y_min = 2000,
			.y_max = 3500,
			.x_deadzone = 350,
			.y_deadzone = 350
		},
		{ // RIGHT JOYSTICK
			.config = {
				.button_type = BUTTON_TYPE_DIGITAL
			},
			.button_up = {
				.scan_code = 'w'-'a'+4
			},
			.button_down = {
				.scan_code = 's'-'a'+4
			},
			.button_left = {
				.scan_code = 'a'-'a'+4
			},
			.button_right = {
				.scan_code = 'd'-'a'+4
			},
			.x_min = 2000,
			.x_max = 4000,
			.y_min = 2000,
			.y_max = 4000,
			.x_deadzone = 350,
			.y_deadzone = 350
			
		}
	}
};