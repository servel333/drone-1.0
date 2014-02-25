/*
 * hid.h
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

#ifndef HID_H_
#define HID_H_

#include <stdint.h>

enum HIDModifier {
	MOD_NONE    = 0x00,
	MOD_L_CTRL  = 0x01,
	MOD_L_SHIFT = 0x02,
	MOD_L_ALT   = 0x04,
	MOD_L_GUI   = 0x08,
	MOD_R_CTRL  = 0x10,
	MOD_R_SHIFT = 0x20,
	MOD_R_ALT   = 0x40,
	MOD_R_GUI   = 0x80
};

struct HIDReport {
	uint8_t modifier; // See enum HIDModifier
	uint8_t scan_code;
};

struct HIDFlags {
	uint16_t number_of_paired_hosts :3;
	uint16_t send_reports_over_uart :1;
	uint16_t descriptor_type        :4;
	uint16_t iOS_virtual_keyboard   :1;
	uint16_t force_hid_mode         :1;
};

#endif /* HID_H_ */