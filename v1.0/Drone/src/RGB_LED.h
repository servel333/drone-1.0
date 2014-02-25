/*
 * RGB_LED.h
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

#ifndef RGB_LED_H_
#define RGB_LED_H_

#include <stdint.h>
#include <asf.h>

#define LED_HIGH 80
#define LED_ON 50
#define LED_DIM 1
#define LED_OFF 0

void setupLEDs(bool swapBlueForGreen);
void setRedLEDValue(uint8_t percent);
void setGreenLEDValue(uint8_t percent);
void setBlueLEDValue(uint8_t percent);
void setLEDValue(uint8_t r, uint8_t g, uint8_t b);

#endif /* RGB_LED_H_ */