/*
 * circular_buffer.h
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

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define CB_STRING_TERMINATOR '\0'

struct CircularBuffer {
	volatile uint8_t* buffer;
	volatile uint8_t front;
	volatile uint8_t back;
	volatile uint8_t count;
	uint8_t size;
};

// Initializes a circular buffer.
void initCB(struct CircularBuffer* cb, uint8_t* buffer, uint8_t size);

// Adds a byte to the circular buffer.  Returns true if successful.
bool addByteToCB(struct CircularBuffer* cb, uint8_t data);

// Adds bytes to the circular buffer.  Returns the number of bytes inserted.
uint8_t addToCB(struct CircularBuffer* cb, uint8_t* src, uint8_t count);

// Returns the number of elements in the circular buffer.
static inline uint8_t getCBCount(struct CircularBuffer* cb) {
	return cb->count;
}

// Clears the circular buffer.
static inline void flushCB(struct CircularBuffer* cb) {
	cb->count = 0;
	cb->back = cb->front;
}

// Reads a byte from the circular buffer.  Returns true if successful.
bool readByteFromCB(struct CircularBuffer* cb, uint8_t *dst);

// Reads, at most, max_count bytes from the circular buffer and returns the
// number of bytes returned.
uint8_t readFromCB(struct CircularBuffer* cb, uint8_t* dst, uint8_t max_count);

// Reads a string from the circular buffer ('\0' terminated).  Returns
// true if a string is found and can fit in dst.
bool readStringFromCB(struct CircularBuffer *cb, char* dst, uint8_t max_count);

#endif /* CIRCULAR_BUFFER_H_ */