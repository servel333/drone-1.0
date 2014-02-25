/*
 * bios.c
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

#include "shared.h"
#include "buttons.h"
#include "joysticks.h"
#include "RGB_LED.h"
#include "watchdog.h"
#include "system_time.h"
#include "configurations.h"
#include "powerControl.h"
#include "shared.h"
#include "analog.h"

#include "usb.h"

#include "bluetooth.h"

#define BIOS_UNUSED_MS 150000

struct GlobalHeader global_header;  // The current global header.
struct Timeout biosUnused; 

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

static struct ControllerConfiguration testModeControllerConfiguration;

void biosMain(void);
void ManufactureTestButtonPress(void);


//Example of new joystick single-ended 12-bit ADC raw values:
//
// x_left: +3400
// x_center: +1940 to +1946
// x_right: +430
//
// y_up: +3330 to +3360
// y_center: +1744 to +1749
// y_down: +449 to +370
//
// conclusion: approximate range of 3000 (unsigned), +-1500 (signed)
// as of 2013-DEC-05, drone_gamepad_config.c deadzones are 300 ; 300/1500 = 20%
#define JOYSTICK_TEST_DEADZONE_X 1000
#define JOYSTICK_TEST_DEADZONE_Y 1000

#define TEST_MODE_NUM_BLINKS 3

const static uint16_xy * center[2] = {(uint16_xy*)&(joysticks[JOYSTICK_LEFT].center_x),(uint16_xy*)&(joysticks[JOYSTICK_RIGHT].center_x)};
const static int16_xy deadzone = {(int16_t)JOYSTICK_TEST_DEADZONE_X,(int16_t)JOYSTICK_TEST_DEADZONE_Y};

void biosMain(void)
{
	//Follows same left,right ordering defined in JoystickName
	//uint16_xy * center[2] = {&(joysticks[JOYSTICK_LEFT].center_x),&(joysticks[JOYSTICK_RIGHT].center_x)};
	//deadzone->x = JOYSTICK_TEST_DEADZONE_X;
	//deadzone->y = JOYSTICK_TEST_DEADZONE_Y;
	
	setupLEDs(false);
	
	for( int i=0; i<TEST_MODE_NUM_BLINKS; i++)
	{
		setLEDValue(LED_DIM, LED_OFF, LED_OFF);
		delay_ms(200);
		setLEDValue(LED_OFF, LED_OFF, LED_OFF);
		delay_ms(200);
	}
	
	turnBTOn(); //Joysticks are on the same power rail island as the Bluetooth module, so turn BT on
	
	ManufactureTestButtonPress();
}


void ManufactureTestButtonPress(void)
{
	startTimeout(&biosUnused, BIOS_UNUSED_MS);
	while(1)
	{
		uint8_t red = LED_OFF;
		uint8_t green = LED_OFF;
		uint8_t blue = LED_OFF;		
		
		WatchdogPet();
		
		if (buttonActivated(B_START) & buttonActivated(B_SELECT))
		{
			delay_ms(500);
			reset_do_soft_reset();
		}
		
		if(checkTimeout(&biosUnused))
		{
			cancelTimeout(&biosUnused);
			setLEDValue(0, 0, 0);
			powerOff(global_header.powerOffSoftware);
		}

		if( buttonActivated(B_UP) ||
			buttonActivated(B_DOWN) ||
			buttonActivated(B_LEFT) ||
			buttonActivated(B_RIGHT) ||
			buttonActivated(B_A) ||
			buttonActivated(B_B) ||
			buttonActivated(B_X) ||
			buttonActivated(B_Y) ||
			buttonActivated(B_START) ||
			buttonActivated(B_SELECT) ||
			buttonActivated(B_LB) ||
			buttonActivated(B_RB) ||
			buttonActivated(B_LT) ||
			buttonActivated(B_RT) ||
			buttonActivated(B_JL) ||
			buttonActivated(B_JR)
		)
		{
			green = LED_DIM;
		}
		
		for(uint8_t idx = 0 ; idx<NUM_JOYSTICKS; idx++) 
		{
			uint16_xy raw;
			raw.x = sampleAnalogChannel(joysticks[idx].adc_channel_x);
			raw.y = sampleAnalogChannel(joysticks[idx].adc_channel_y);
			
			int16_xy out;
			SimpleJoystickAdj( center[idx], &deadzone, &raw, &out );
			
			//Write raw and adjusted X,Y values to USB serial port
			PrintXY(raw.x,raw.y);
			sendUSBString("->", false);
			PrintXY(out.x,out.y);
			sendUSBLine("");
			
			if( out.y<0 )
			{
				//up is yellow
				red = LED_DIM;
				green = LED_DIM;
			}
			else if( out.y>0 )
			{
				//down is blue
				blue = LED_DIM;
			}
			else if( out.x>0 )
			{
				//right is Red
				red = LED_DIM;
			}
			else if( out.x<0 )
			{
				//left is green
				green = LED_DIM;
			}
		}
		
		setLEDValue(red, green, blue);
				
	} // end while(1)
}
