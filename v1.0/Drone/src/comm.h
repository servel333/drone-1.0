/*
 * comm.h
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

#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>

struct Command {
	const char* CMD;
	void (*action)(char* opts);
};

// Converts a byte to two hexadecimal (0-9, a-f) digits.
void byteToASCIIHex(const uint8_t b, char* high, char* low);

// Converts two hexadecimal (0-9, a-f) digits to a byte.
uint8_t ASCIIHexToByte(char high, char low);

// Look for a command and execute it.
void processComm(void);

#endif /* COMM_H_ */