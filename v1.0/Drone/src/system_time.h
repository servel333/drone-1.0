/*
 * system_time.h
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

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_

struct Timeout {
	uint32_t start;
	uint32_t delay;
};

// Safely gets the system time.
uint32_t getSystemTime(void);

// Add a tick to the system time. (Should be called in the systick callback).
void tickSystemTime(void);

// Used internal to system_time, the functionality is available through startTimeout
// Sets the system time.
//void setSystemTime(uint32_t sys_time);

// Start a timeout counter.
void startTimeout(struct Timeout* to, uint32_t ms);

// Check to see if a timeout has occurred.
// Returns true if the timeout has happened; false otherwise.
bool checkTimeout(struct Timeout* to);

// Cancels a timeout. (Sets the delay to 0).
void cancelTimeout(struct Timeout* to);

#endif /* SYSTEM_TIME_H_ */