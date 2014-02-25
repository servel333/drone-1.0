/*
 * systick.c
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
#include "systick.h"

void setupSysTick(tc_callback_t tick) {
	
	// Make sure systick is enabled before trying to making changes:
	enableSysTick();
	
	// Set the counting direction:
	tc_set_direction(SYSTICK_TIMER, TC_UP);
	
	// Set the waveform type:
	tc_set_wgm(SYSTICK_TIMER, TC_WG_NORMAL);
	
	// Reset the counter:
	tc_write_count(SYSTICK_TIMER, 0);
	
	// Set the period:
	tc_write_period(SYSTICK_TIMER, F_CPU/SYSTICK_FREQUENCY);

	// Clear the overflow flag:
	clearSysTickFlag();
	
	// Set the callback:
	tc_set_overflow_interrupt_callback(SYSTICK_TIMER, tick);
	
	// Set the interrupt priority:
	tc_set_overflow_interrupt_level(SYSTICK_TIMER, TC_INT_LVL_LO);

	// Select the clock source (starts counting):
	tc_write_clock_source(SYSTICK_TIMER, TC_CLKSEL_DIV1_gc);

	// Leave it to be enabled later:
	disableSysTick();
}

void enableSysTick(void) {
	tc_enable(SYSTICK_TIMER);
}

void disableSysTick(void) {
	tc_disable(SYSTICK_TIMER);
}

void clearSysTickFlag(void) {
	tc_clear_overflow(SYSTICK_TIMER);
}
