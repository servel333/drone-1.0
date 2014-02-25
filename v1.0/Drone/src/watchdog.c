/*
 * watchdog.c
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

/** @file
* This file contains the code for initializing and petting the watchdog
* If the watchdog is not petted within the timeout period, the processor
* is reset and the code runs as if the unit just powered up.
*
* @author Alan Widmer
*/

#include "watchdog.h"
#include "debug.h"

/* Add global watchdog down-counters here */
static uint16_t modemResponseWatchdog = WATCHDOG_INITIALIZE_RESET;
static uint16_t mainLoopWatchdog = WATCHDOG_INITIALIZE_RESET;

/**
* Sets up the watchdog for a timeout period of 8 seconds.
* The function WatchdogPet must be called at least once every 8 seconds
* otherwise the processor will be reset.
* Normally the application code will call WatchdogValidate() much more
* frequently than once per 8 seconds and the validate function will check all
* subsystems for continuing operation.
*/
void WatchdogInit(void)
{
	// Setup prescaler
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_8KCLK);
	#if NO_WATCHDOG != 1
	/* Now enable the watchdog */
	wdt_enable();
	#else
	#ifndef DEBUG
	#error "Watchdog must be enabled in release build. Don't define NO_WATCHDOG"
	#endif
	#endif
}

/**
* Reset the hardware watchdog so it does not timeout and restart the
* processor.
*/
void WatchdogPet(void)
{
	sendDBGString(".");
	wdt_reset();
}


/**
* Check that each subsystem within the application is still running and then
* if everything is OK, reset the watchdog countdown timer to prevent a
* restart. If one of the subsystems has failed, allow the watchdog to timeout
* and force a restart.
* @note This function is called once per second from the system timer.
*/
void WatchdogValidate(void)
{
	bool systemStalled = false;

	/* Main loop still running? */
	if (mainLoopWatchdog == 0)
	{
		// TODO re-enable this once wdt is debugged
		// sendDBGLine("TASK_MAIN watchdog task timeout");
		// systemStalled = true;
	}
	else
	{
		mainLoopWatchdog--;
	}

	if (!systemStalled)
	{
		WatchdogPet();
	}
}

void WatchDogEnable(bool enable)
{
	if (enable)
	{
		wdt_enable();
	}		
	else
	{
		wdt_disable();
	}			
}

/**
* The function is called by each task in the system that could conceivably
* fail independently.
* @param task The task that is calling to confirm that it is still running.
*/
void WatchdogTaskRunning(taskDef task)
{
	switch (task)
	{
		case TASK_MAIN:
		mainLoopWatchdog = MAIN_LOOP_WATCHDOG_RESET;
		break;
		case TASK_MODEM:
		modemResponseWatchdog = MODEM_RESPONSE_WATCHDOG_RESET;
		break;
	}
}
