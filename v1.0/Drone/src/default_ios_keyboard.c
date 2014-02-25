/*
 * default_ios_keyboard.c
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

const struct ControllerConfiguration DEFAULT_IOS_CONFIG = {
	.mode = BTM_IOS_KEYBOARD,
	.button_configs = {
		{ // UP
			.button_down = {
				.scan_code = 'w'-'a'+4
			},
			.button_up = {
				.scan_code = 'e'-'a'+4
			}
		},
		{ // DOWN
			.button_down = {
				.scan_code = 'x'-'a'+4
			},
			.button_up = {
				.scan_code = 'z'-'a'+4
			}
		},
		{ // LEFT
			.button_down = {
				.scan_code = 'a'-'a'+4
			},
			.button_up = {
				.scan_code = 'q'-'a'+4
			}
		},
		{ // RIGHT
			.button_down = {
				.scan_code = 'd'-'a'+4
			},
			.button_up = {
				.scan_code = 'c'-'a'+4
			}
		},
		{ // A
			.button_down = {
				.scan_code = 'l'-'a'+4
			},
			.button_up = {
				.scan_code = 'v'-'a'+4
			}
		},
		{ // B
			.button_down = {
				.scan_code = 'o'-'a'+4
			},
			.button_up = {
				.scan_code = 'g'-'a'+4
			}
		},
		{ // X
			.button_down = {
				.scan_code = 'k'-'a'+4
			},
			.button_up = {
				.scan_code = 'p'-'a'+4
			}
		},
		{ // Y
			.button_down = {
				.scan_code = 'i'-'a'+4
			},
			.button_up = {
				.scan_code = 'm'-'a'+4
			}
		},
		{ // START
			.button_down = {
				.scan_code = 'u'-'a'+4
			},
			.button_up = {
				.scan_code = 'f'-'a'+4
			}
		},
		{ // SELECT
			.button_down = {
				.scan_code = 'y'-'a'+4
			},
			.button_up = {
				.scan_code = 't'-'a'+4
			}
		},
		{ // RB
			.button_down = {
				.scan_code = 'j'-'a'+4
			},
			.button_up = {
				.scan_code = 'n'-'a'+4
			}
		},
		{ // LB
			.button_down = {
				.scan_code = 'h'-'a'+4
			},
			.button_up = {
				.scan_code = 'r'-'a'+4
			}
		},
		{ // RT
			.button_down = {
				.scan_code = 'j'-'a'+4
			},
			.button_up = {
				.scan_code = 'n'-'a'+4
			}
		},
		{ // LT
			.button_down = {
				.scan_code = 'h'-'a'+4
			},
			.button_up = {
				.scan_code = 'r'-'a'+4
			}
		}
	}
};