/*
 * circular_buffer.c
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

#include "circular_buffer.h"
#include <asf.h>

#define MIN(x,y) (((x) < (y))? (x) : (y))

void initCB(struct CircularBuffer* cb, uint8_t* buffer, uint8_t size) {
	cb->buffer = buffer;
	cb->size   = size;
	cb->front  = 0;
	flushCB(cb);
}

bool addByteToCB(struct CircularBuffer* cb, uint8_t data) {
	bool success = false;
	// Save the interrupt state and disable interrupts:
	irqflags_t flags = cpu_irq_save();
	
	// Is there room in the buffer?
	if (cb->count < cb->size) {
		// Add the data to the front and update counters:
		cb->buffer[cb->front++] = data;
		cb->count++;
		
		// Has the front value overflown?
		if (cb->front >= cb->size) {
			cb->front = 0;
		}
		
		// Hooray!
		success = true;
	}

	// Restore the interrupt state:
	cpu_irq_restore(flags);
	
	return success;
}

uint8_t addToCB(struct CircularBuffer* cb, uint8_t* src, uint8_t count) {
	uint8_t temp = 0;
	
	while (temp < count && addByteToCB(cb, src[temp])) {
		temp++;
	}
	
	return temp;
}

bool readByteFromCB(struct CircularBuffer* cb, uint8_t *dst) {
	bool success = false;
	// Save the interrupt state and disable interrupts:
	irqflags_t flags = cpu_irq_save();

	// Is there available data?
	if (cb->count > 0) {
		// Remove the data from the back and update counters:
		*dst = cb->buffer[cb->back++];
		cb->count--;
		
		// Has the back value overflown?
		if (cb->back >= cb->size) {
			cb->back = 0;
		}
		
		// Hooray!
		success = true;
	}

	// Restore the interrupt state:
	cpu_irq_restore(flags);
	
	return success;
}

uint8_t readFromCB(struct CircularBuffer* cb, uint8_t* dst, uint8_t max_count) {
	uint8_t temp = 0;
		
	while (temp < max_count && readByteFromCB(cb, dst + temp)) {
		temp++;
	}		
		
	return temp;
}

bool readStringFromCB(struct CircularBuffer *cb, char* dst, uint8_t max_count) {
	uint8_t temp, idx, count;
	bool found = false;
	// Save the interrupt state and disable interrupts:
	irqflags_t flags = cpu_irq_save();
	
	// Start at the back for the circular buffer:
	idx = cb->back;
	
	// Pick the minimum search area:
	count = MIN(cb->count, max_count);
	
	// Search for the first CB_STRING_TERMINATOR:
	for (temp = 0; temp < count; temp++) {
		// Has it been found?
		if (cb->buffer[idx] == CB_STRING_TERMINATOR) {
			// Then stop searching:
			found = true;
			break;
		}
		
		// Increment:
		idx++;
		if (idx >= cb->size) {
			idx = 0;
		}
	}

	// Restore the interrupt state:
	cpu_irq_restore(flags);
	
	// Make sure there is room for the terminator:
	found &= (temp + 1) < max_count;
	
	// Read the line:
	if (found) {
		readFromCB(cb, dst, temp + 1);
	}
	
	return found;
}
