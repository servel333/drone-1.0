/*
 * analog.c
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
#include "analog.h"
#include "joysticks.h"

static struct adc_config         analog_config;
static struct adc_channel_config analog_channel_config;

void setupAnalog(void) {
	uint8_t i;
	
	// Populate the structs with default values:
	adc_read_configuration(&JOYSTICK_ADC_MODULE, &analog_config);
	adcch_read_configuration(&JOYSTICK_ADC_MODULE, JOYSTICK_ADC_CHANNEL, &analog_channel_config);
	
	// Configure the ADC
	adc_set_conversion_parameters(&analog_config, DRONE_ADC_SIGN,DRONE_ADC_RES,ADC_REF_AREFA); // Reference is VCC/1.6
	adc_set_conversion_trigger(&analog_config, ADC_TRIG_MANUAL, 1, 0);
	
	// Set the adc clock rate:
	adc_set_clock_rate(&analog_config, ADC_CLOCK_HZ);

	// Write the configuration to its registers:
	adc_write_configuration(&JOYSTICK_ADC_MODULE, &analog_config);
}

void AdcInput(enum adcch_positive_input channel)
{
	// Select the input to the adc:
	adcch_set_input(&analog_channel_config, channel, ADCCH_NEG_NONE, 1);

	// Write the channel configuration:
	adcch_write_configuration(&JOYSTICK_ADC_MODULE, JOYSTICK_ADC_CHANNEL, &analog_channel_config);
};

//THIS IS BLOCKING!
uint16_t AdcSample(void)
{
	// Start the conversion:
	adc_start_conversion(&JOYSTICK_ADC_MODULE, JOYSTICK_ADC_CHANNEL);
		
	// Wait for it to finish:
	adc_wait_for_interrupt_flag(&JOYSTICK_ADC_MODULE, JOYSTICK_ADC_CHANNEL); //THIS IS BLOCKING!

	// Return the result
	return (adc_get_result(&JOYSTICK_ADC_MODULE, JOYSTICK_ADC_CHANNEL)); //was flipping by inverting result
}

uint16_t sampleAnalogChannel(enum adcch_positive_input channel) {
	uint16_t result;
	AdcInput(channel);
	result = AdcSample();
	return result;
};

void enableAnalog(void)
{
	adc_enable(&JOYSTICK_ADC_MODULE);
}

void disableAnalog(void)
{
	adc_disable(&JOYSTICK_ADC_MODULE);
}