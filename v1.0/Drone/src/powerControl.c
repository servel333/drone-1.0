/*
* powerControl.c
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

#include "asf.h"
#include "buttons.h"
#include "debug.h"
#include "bluetooth.h"
#include "powerControl.h"
#include "system_time.h"
#include "watchdog.h"
#include "drone_game_configurations.h"

#define STARTUP_HOLD_TIME_MS 2000

//declared in main.c
extern struct ControllerConfiguration ccc; // Current Controller Configuration.
extern struct GlobalHeader global_header;  // The current global header.

bool Wakeup(uint16_t timeout_ms);

void powerOff(bool powerOffSoftware) 
{
	WatchdogPet();
	WatchDogEnable(false);
	turnBTOff();
	
	delay_ms(20);
	cpu_irq_enable();	

	sleep_set_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	
	while(1)
	{
			sleep_enter();
			
			//EXECUTION STARTS HERE AFTER START_PIN EXTERNAL INTERRUPT ISR
			
			if( Wakeup(STARTUP_HOLD_TIME_MS) )
			{
				break; //don't reenter sleep
			}
	}
	
	sleep_disable();
	reset_do_soft_reset();
}

bool Wakeup(uint16_t timeout_ms)
{
	
	cfgsel_t which_cfg;
	struct Timeout t;
	startTimeout( &t, timeout_ms );
	which_cfg = whichConfiguration();

	while( ! checkTimeout(&t) )
	{
		if( ! buttonActivated(B_START) )
		{
			return false;
		}
	}
	loadConfiguration(which_cfg);
	return true;
}

// Start Button must be held down for timeout_ms
// Returns True for Startup
// Returns False for PowerOff
bool checkForStartUp(uint16_t timeout_ms) {
	// Loop and Monitor START_PIN to make sure it's pressed.
	// Return value when timeout has been reached. 
	WatchdogPet();	
	WatchDogEnable(false);
	uint32_t count = getSystemTime();

	while (ioport_get_pin_level(START_PIN)); 
	{
		delay_ms(10);		
	} 
	
	bool turn_on = (count + timeout_ms) > getSystemTime();

	WatchDogEnable(true); 
	WatchdogPet();
	return turn_on;
}

// Parameter is taken from the Global EEPROM Header
// Returns when Start button held long enough
// Else puts the Controller to sleep
bool powerOn(bool powerOffSoftware) {
	
	// Otherwise we go to sleep
	if (!checkForStartUp(STARTUP_HOLD_TIME_MS))
	{
		// Startup is NOT going to happen
		// Go back to sleep
		powerOff(powerOffSoftware);
		
	}
	return true;
}

