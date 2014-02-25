/*
 * system_time.c
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

#include <asf.h>
#include "system_time.h"

static volatile uint32_t system_time = 0;

// Returns the number of times the timer has ticked (1000 Hz)
uint32_t getSystemTime(void) {
	uint32_t temp;
	irqflags_t irq_state;
	
	// Save and disable interrupts:
	irq_state = cpu_irq_save();

	// Get the system time:
	temp = system_time;
	
	// Restore the state of the interrupts:
	cpu_irq_restore(irq_state);
	
	return temp;
}

// Increment the counter when the timer ticks
void tickSystemTime(void) {
	irqflags_t irq_state;
		
	// Save and disable interrupts:
	irq_state = cpu_irq_save();

	// Increment the system time:
	system_time++;
		
	// Restore the state of the interrupts:
	cpu_irq_restore(irq_state);
}

// Set the initial value to something other than zero.
void setSystemTime(uint32_t sys_time) {
	irqflags_t irq_state;
	
	// Save and disable interrupts:
	irq_state = cpu_irq_save();

	// Set the system time:
	system_time = sys_time;
	
	// Restore the state of the interrupts:
	cpu_irq_restore(irq_state);
}

// Initialize the Timeout Struct with the current System Tick Count and the Milliseconds delay
void startTimeout(struct Timeout* to, uint32_t ms) {
	to->start = getSystemTime();
	to->delay = ms;
}

// True if not Cancelled AND Current System Tick Count >= Delay Count
// False if Cancelled OR Current System Tick Count < Delay
bool checkTimeout(struct Timeout* to) {
	return (to->delay != 0) && ((getSystemTime() - to->start) >= to->delay);
}

// Set the Delay to 0 to cancel the timeout.
void cancelTimeout(struct Timeout* to) {
	to->delay = 0;
}