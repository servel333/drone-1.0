/*
 * battery_charger.c
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
#include "battery_charger.h"
#include "RGB_LED.h"

#include "bluetooth.h"
#include "ioport.h"
#include "analog.h"
#include "powerControl.h"
#include "configurations.h"

#include <sleep.h>

extern struct GlobalHeader global_header;  // The current global header.

#define PIN_CHECK_DELAY_US 20

/*
//R19=7.5kOhm and R20=10kOhm doesn't allow getChargeState() to differentiate between 0 (charging) and HiZ (off)
ChargeState_t getChargeState()
{
	//Charge Cycle State - MCP73831
	//Shutdown             Hi-Z
	//No Battery Present   Hi-Z
	//Preconditioning      L
	//Constant-Current     L
	//Constant Voltage     L
	//Charge Complete      H
	
	ChargeState_t toRet = CHARGER_ERROR;
	
	// Check for HIGH state:
	// Enable the pull-down and look for a high state:
	ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_PULLDOWN);
	delay_us(PIN_CHECK_DELAY_US);
	
	// Is the pin still high?
	if (ioport_get_pin_level(CHRG_STAT_PIN) == 1) {
		toRet = NOT_CHARGING;
	}
	else {
		// Next, check for low state:
		// Enable the pull-up and look for a low state:
		ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_PULLUP);
		
		delay_us(PIN_CHECK_DELAY_US);
		
		// Is the pin still low?
		if (ioport_get_pin_level(CHRG_STAT_PIN) == 0) {
			toRet = CHARGING;
		}
	}
	
	// Return the pin to it's normal setup:
	ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_TOTEM);

	// If it wasn't high or low, then it must be hi-z:
	return toRet;
}
*/


bool IsChargingDone(void)
{
	bool chargingDone;
	ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_PULLDOWN); //set pulldown mode
	delay_us(PIN_CHECK_DELAY_US);
	chargingDone = ioport_get_pin_level(CHRG_STAT_PIN);
	ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_TOTEM); //set pin to normal (low power) mode
	return chargingDone;
}


//VREF_EXT = 3.1*(10/(10+2.4))=2.5V
//BAT_DET = V_BAT*(10/(10+7.5))
//Count = BAT_DET*4096/VREF_EXT
//Counts:

//From measured values (See battery.xlsx)
#define BAT_DET_3P35V 3129
#define BAT_DET_3P40V 3139
#define BAT_DET_3P45V 3169
#define BAT_DET_3P50V 3210
#define BAT_DET_3P55V 3259
#define BAT_DET_3P60V 3294
#define BAT_DET_3P65V 3338
#define BAT_DET_3P70V 3374
#define BAT_DET_3P75V 3422
#define BAT_DET_3P80V 3465
#define BAT_DET_3P85V 3505
#define BAT_DET_3P90V 3549
#define BAT_DET_3P95V 3596
#define BAT_DET_4P00V 3637
#define BAT_DET_4P05V 3682
#define BAT_DET_4P10V 3724
#define BAT_DET_4P15V 3772
#define BAT_DET_4P20V 3813

//Single Cell Lipo 0% is 3.00V
//Single Cell Lipo 100% is 4.20V

//Drone 0% is 3.35V due to regulators (ADC can't consistently read below this value)
//Built in safety margin of 0.35V
//Drone 100% is 4.20V
//Drone 5% ~ 3.40V
//Drone 15% ~ 3.50V

const uint16_t BATTERY_SAFE_THRESHOLD = BAT_DET_3P35V;
const uint16_t BATTERY_LOW_THRESHOLD  = BAT_DET_3P40V; //~5%
const uint16_t BATTERY_MED_THRESHOLD  = BAT_DET_3P50V; //~15%
const uint16_t BATTERY_HIGH_THRESHOLD = BAT_DET_4P10V; //~85%

const uint8_t BATTERY_DEAD_INDICATOR = 3;

void MaintainControllerStatus(bool pulse, bool chargingDone, uint16_t battery_level)
{
	bool bluetoothConnected = btIsConnected();
	
	uint8_t r = LED_OFF;
	uint8_t g = LED_OFF;
	uint8_t b = LED_OFF;
	
	//ChargeState_t batteryChargeState = !ioport_get_pin_level(CHRG_STAT_PIN) ? FULLY_CHARGED : CHARGING;
	//ChargeState_t batteryChargeState = getChargeState();
	//batteryChargeState = NOT_CHARGING; //TODO: getChargeState() can't differentiate between 0 and HiZ -- comment this to reenable

	if(chargingDone)
	{
		r = g = b = LED_DIM; // White
	}
	else // Not Charging
	{
		if(battery_level < BATTERY_SAFE_THRESHOLD)
		{
			for( uint8_t i=0; i<BATTERY_DEAD_INDICATOR; i++)
			{
				setLEDValue(LED_DIM, LED_OFF, LED_OFF);
				delay_ms(200);
				setLEDValue(LED_OFF, LED_OFF, LED_OFF);
				delay_ms(200);
			}
			powerOff(global_header.powerOffSoftware);
		}
		else if (battery_level < BATTERY_LOW_THRESHOLD)
		{
			//Needs Charging
			r = LED_DIM; // Red
		}
		else if(battery_level < BATTERY_MED_THRESHOLD)
		{
			//Needs Charging
			r = g = LED_DIM; // Yellow
		}
		else
		{
			// Normal - Ideal State
			b = LED_DIM; // Default to blue
		}
	}
	//If Bluetooth not connected, blink
	if (!bluetoothConnected && !pulse)
	{
		r = g = b = LED_OFF;
	}
	
	setLEDValue(r,g,b);
}


//Setup CHRG_STAT_PIN external interrupt
void SetupChrgStatPinExternalInterrupt(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	
	PORTC_INT0MASK = 1u<<0; //Set PORTC interrupt on PORTC.0 pin
	arch_ioport_set_pin_sense_mode(CHRG_STAT_PIN,IOPORT_SENSE_RISING);
	PORTC_INTCTRL = 2u; //Enable medium level external interrupt
	
	cpu_irq_restore(flags);
}

// PORTF interrupt vector, Vector 0 is the reset vector
ISR(PORTC_INT0_vect)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	
	// If interrupt0 flag is set, clear it
	#define INT0IF 1u<<0
	#define INT1IF 1u<<1
	if( PORTC_INTFLAGS & INT0IF )
	{
		PORTC_INTFLAGS = INT0IF; // write-1-to-clear - to clear interrupt flag write a '1'
	}
    
	setLEDValue(LED_DIM,LED_DIM,LED_DIM);
	
	if( SLEEP_MODE_PWR_DOWN==(SLEEP.CTRL & SLEEP_SMODE_gm) )
	{
		sleep_disable();
		reset_do_soft_reset(); // don't want to continue in powerOff() - checks that the START button was pressed
	}
	
	cpu_irq_restore(flags);
}




