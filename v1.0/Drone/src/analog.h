/*
 * analog.h
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

#ifndef ANALOG_H_
#define ANALOG_H_


//Resolution= ADC_RES_8|ADC_RES_12|ADC_RES_12_LEFT
//Sign= ADC_SIGN_OFF|ADC_SIGN_ON
//8-bit signed:    -127 to +127
//8-bit unsigned:  0 to 255
//12-bit signed:   -2047 to +2047
//12-bit unsigned: 0 to 4095

//ADC=Unsigned or Unsigned
//#define DRONE_ADC_SIGN ADC_SIGN_ON
#define DRONE_ADC_SIGN ADC_SIGN_OFF

//ADC Resolution
//#define DRONE_ADC_RES ADC_RES_8
#define DRONE_ADC_RES ADC_RES_12
//#define DRONE_ADC_RES ADC_RES_12_LEFT

#if DRONE_ADC_RES==ADC_RES_12 && DRONE_ADC_SIGN==ADC_SIGN_ON
	#define IDEAL_ADC_MIN -2047
	#define IDEAL_ADC_MAX  2047
#elif DRONE_ADC_RES==ADC_RES_12 && DRONE_ADC_SIGN==ADC_SIGN_OFF
    #define IDEAL_ADC_MIN 0
	#define IDEAL_ADC_MAX 4095
#elif DRONE_ADC_RES==ADC_RES_8 && DRONE_ADC_SIGN==ADC_SIGN_ON
	#define IDEAL_ADC_MIN -127
	#define IDEAL_ADC_MAX  127
#elif DRONE_ADC_RES==ADC_RES_8 && DRONE_ADC_SIGN==ADC_SIGN_OFF
	#define IDEAL_ADC_MIN  0
	#define IDEAL_ADC_MAX  255
#else
	#error "Unsupported ADC configuration"
#endif

#define DRONE_ADC_ROUGH_OFFSET ((IDEAL_ADC_MAX-IDEAL_ADC_MIN)/8)
#define DRONE_ADC_MIN (IDEAL_ADC_MIN + DRONE_ADC_ROUGH_OFFSET)
#define DRONE_ADC_MAX (IDEAL_ADC_MAX - DRONE_ADC_ROUGH_OFFSET)
#define DRONE_ADC_CENTER ((DRONE_ADC_MAX-DRONE_ADC_MIN)/2)

//#define ADC_CLOCK_HZ 200000UL
#define ADC_CLOCK_HZ 500000UL

void setupAnalog(void);

void AdcInput(enum adcch_positive_input channel);
uint16_t AdcSample(void);

uint16_t sampleAnalogChannel(enum adcch_positive_input channel);
void enableAnalog(void);
void disableAnalog(void);

#endif /* ANALOG_H_ */
