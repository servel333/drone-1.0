/*
 * RGB_LED.c
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

#include "RGB_LED.h"
#include <asf.h>

// Locals:
enum led_config {
	RED_CFG = 0,
	GREEN_CFG,
	BLUE_CFG
};

/* The 3 PWM config structs */
static struct pwm_config pwm_cfg[3];

static bool swapBlueGreen = false;  // Default = 0/false behavior is Green on D0 and Blue on D1, 

void setupLEDs(bool swapBlueForGreen)
{
	swapBlueGreen = swapBlueForGreen;
	/* Set up all 3 PWM channels with 500 Hz frequency. */
	pwm_init(&pwm_cfg[RED_CFG],   RED_PWM_PORT,   RED_PWM_CHANNEL,   500);
	pwm_init(&pwm_cfg[GREEN_CFG], GREEN_PWM_PORT, GREEN_PWM_CHANNEL, 500);
	pwm_init(&pwm_cfg[BLUE_CFG],  BLUE_PWM_PORT,  BLUE_PWM_CHANNEL,  500);

	/* Start all 3 PWM channels with initial value of off.
	This is the only location that will need to reference them
	as active low (100% == off) */
	pwm_start(&pwm_cfg[RED_CFG], 100);
	pwm_start(&pwm_cfg[GREEN_CFG], 100);
	pwm_start(&pwm_cfg[BLUE_CFG], 100);
}

void setRedLEDValue(uint8_t percent)
{
	if (percent > 100) {
		percent = 100;
	}
	pwm_set_duty_cycle_percent(&pwm_cfg[RED_CFG], 100 - percent);
}

void setGreenLEDValue(uint8_t percent)
{
	if (percent > 100) {
		percent = 100;
	}
	pwm_set_duty_cycle_percent(&pwm_cfg[ swapBlueGreen ? BLUE_CFG : GREEN_CFG], 100 - percent);
}

void setBlueLEDValue(uint8_t percent)
{
	if (percent > 100) {
		percent = 100;
	}
	pwm_set_duty_cycle_percent(&pwm_cfg[ swapBlueGreen ? GREEN_CFG : BLUE_CFG ], 100 - percent);
}

void setLEDValue(uint8_t r, uint8_t g, uint8_t b)
{
	setRedLEDValue(r);
	setGreenLEDValue(g);
	setBlueLEDValue(b);
}