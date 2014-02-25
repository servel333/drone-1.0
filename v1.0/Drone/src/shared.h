/*
 * shared.h
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

#ifndef SHARED_H_
#define SHARED_H_
#include "asf.h"

// http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=40338
#define BIT(bit) (0x01 << (bit))
#define BIT_GET(bite, bit) ((bite) & (bit))
#define BIT_HIGH(bite, bit) ((bite) |= (bit))
#define BIT_LOW(bite, bit) ((bite) &= ~(bit))
#define BIT_TOGGLE(bite, bit) ((bite) ^= (bit))
#define BIT_SET(bite,pin,val) (val ? BIT_HIGH(bite,pin) : BIT_LOW(bite,pin))

void bootloader(void);

int32_t map(int32_t y, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);


// http://embeddedgurus.com/stack-overflow/2012/02/the-absolute-truth-about-abs/
//int nabs(int i)
//{
//	return i > 0 ? -i : i;
//}
#define NABS(i) ( ((i)>0) ? -(i) : (i) )

//mirror across midpoint between in_min and in_max
//#define FLIPPED_MAP(X,IN_MIN,IN_MAX,OUT_MIN,OUT_MAX) map((-(X)),(IN_MIN),(IN_MAX),(-(OUT_MIN)),(OUT_MAX))


typedef struct {uint16_t x; uint16_t y;} uint16_xy;
typedef struct {int16_t x;  int16_t y;}  int16_xy;
typedef struct {int8_t x;   int8_t y;}   int8_xy;


#endif /* SHARED_H_ */