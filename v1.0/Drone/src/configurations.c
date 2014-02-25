/*
 * configurations.c
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
#include "bluetooth.h"
#include "configurations.h"
#include "drone_game_configurations.h"
#include "debug.h"
#include "bios.h"
#include "RGB_LED.h"
#include <string.h>

// Declared in main.c
extern struct ControllerConfiguration ccc; // Current Controller Configuration.
extern struct GlobalHeader global_header;  // The current global header.

// Constants:
#define GLOBAL_HEADER_EEPROM_ADDRESS EEPROM_START
#define GLOBAL_HEADER_EEPROM_PAGE_COUNT ((sizeof(struct GlobalHeader) + (EEPROM_PAGE_SIZE-1)) / EEPROM_PAGE_SIZE)
#define GLOBAL_HEADER_EEPROM_SIZE (GLOBAL_HEADER_EEPROM_PAGE_COUNT * EEPROM_PAGE_SIZE)
#define CONTROLLER_CONFIGURATION_EEPROM_START_ADDRESS (GLOBAL_HEADER_EEPROM_ADDRESS + GLOBAL_HEADER_EEPROM_SIZE)
#define CONTROLLER_CONFIGURATION_EEPROM_PAGE_COUNT ((sizeof(struct ControllerConfiguration) + (EEPROM_PAGE_SIZE-1)) / EEPROM_PAGE_SIZE)
#define CONTROLLER_CONFIGURATION_EEPROM_SIZE (CONTROLLER_CONFIGURATION_EEPROM_PAGE_COUNT * EEPROM_PAGE_SIZE)

void readGlobalHeader(struct GlobalHeader *gh) {
	nvm_eeprom_read_buffer(GLOBAL_HEADER_EEPROM_ADDRESS, (void *)gh, sizeof(struct GlobalHeader));
}

void writeGlobalHeader(const struct GlobalHeader *gh) {
	nvm_eeprom_erase_and_write_buffer(GLOBAL_HEADER_EEPROM_ADDRESS, (const void *) gh, sizeof(struct GlobalHeader));
}

void readControllerConfig(uint8_t index, struct ControllerConfiguration *cc) {
	eeprom_addr_t addr = CONTROLLER_CONFIGURATION_EEPROM_START_ADDRESS;
	addr += index * CONTROLLER_CONFIGURATION_EEPROM_SIZE;
	nvm_eeprom_read_buffer(addr, (void *)cc, sizeof(struct ControllerConfiguration));
}

void writeControllerConfig(uint8_t index, const struct ControllerConfiguration *cc) {
	eeprom_addr_t addr = CONTROLLER_CONFIGURATION_EEPROM_START_ADDRESS;
	addr += index * CONTROLLER_CONFIGURATION_EEPROM_SIZE;
	nvm_eeprom_erase_and_write_buffer(addr, (const void *)cc, sizeof(struct ControllerConfiguration));
}

void loadConfiguration(cfgsel_t which_configuration) {
	
	if(which_configuration==CFG_SELECT)
	{
		sendDBGLine("BIOS");
		biosMain();
	}
	
	readGlobalHeader(&global_header); // Load the global header:

	// Do the default configurations need to be written?	
	if (global_header.first_run == 1) {

		//Write the default controller configurations to EEPROM:
		writeControllerConfig(CFG_A,     &DRONE_GAMEPAD_CONFIG);		// B_A
		writeControllerConfig(CFG_B,     &DRONE_IOS_CONFIG);			// B_B
		writeControllerConfig(CFG_X,     &DRONE_KEYBOARD_CONFIG);		// B_X
		writeControllerConfig(CFG_Y,     &DRONE_KEYBOARD_MOUSE_CONFIG);	// B_Y
		writeControllerConfig(CFG_RB,    &DRONE_JOYSTICK_CONFIG);		// B_RB
		//writeControllerConfig(CFG_DOWN,  &DRONE_OPEN_CONFIG_1);			// B_DOWN
		//writeControllerConfig(CFG_RIGHT, &DRONE_OPEN_CONFIG_2);			// B_RIGHT
		//writeControllerConfig(CFG_UP,    &DRONE_OPEN_CONFIG_3);			// B_UP
		//writeControllerConfig(CFG_LEFT,  &DRONE_OPEN_CONFIG_4);			// B_LEFT
		//writeControllerConfig(CFG_LB,	   &DRONE_OPEN_CONFIG_5);			// B_LB
		
		// Write the default header to EEPROM:
		memcpy(&global_header, &DEFAULT_HEADER, sizeof(struct GlobalHeader));
		global_header.first_run = 0;
		writeGlobalHeader(&global_header);
		btFactoryReset();
	}
	
	sendDBGString("Loading configuration ");
	sendDBGChar('0' + which_configuration);
	sendDBGString("");
	readControllerConfig(which_configuration, &ccc);
}	
void displayWhichConfiguration(cfgsel_t configuration)
{
	switch (configuration)
	{
		case CFG_SELECT:	setLEDValue(LED_OFF, LED_OFF, LED_OFF); break;
		case CFG_A:			setLEDValue(LED_OFF, LED_OFF, LED_DIM); break;
		case CFG_B:			setLEDValue(LED_DIM, LED_OFF, LED_OFF); break;
		case CFG_X:			setLEDValue(LED_OFF, LED_DIM, LED_OFF); break;
		case CFG_Y:			setLEDValue(LED_DIM, LED_DIM, LED_OFF); break;
		case CFG_RB:		setLEDValue(LED_DIM, LED_OFF, LED_DIM); break;
		case CFG_DOWN:		setLEDValue(LED_OFF, LED_OFF, LED_DIM); break;
		case CFG_RIGHT:		setLEDValue(LED_DIM, LED_OFF, LED_OFF); break;
		case CFG_LEFT:		setLEDValue(LED_OFF, LED_DIM, LED_OFF); break;
		case CFG_UP:		setLEDValue(LED_DIM, LED_DIM, LED_OFF); break;
		case CFG_LB:		setLEDValue(LED_DIM, LED_OFF, LED_DIM); break;		
	}
}

cfgsel_t whichConfiguration(void){
	cfgsel_t which_configuration;
	
	if (buttonActivated(B_SELECT))
	{
		which_configuration = CFG_SELECT;
	}
	else if (buttonActivated(B_A)) {
		which_configuration = CFG_A;
	}
	else if (buttonActivated(B_B)) {
		which_configuration = CFG_B;
	}
	else if (buttonActivated(B_X)) {
		which_configuration = CFG_X;
	}
	else if (buttonActivated(B_Y)) {
		which_configuration = CFG_Y; 
	}
	else if (buttonActivated(B_RB)) {
		which_configuration = CFG_RB;
	}
	else if (buttonActivated(B_DOWN)) {
		which_configuration = CFG_DOWN;
	}
	else if (buttonActivated(B_RIGHT)) {
		which_configuration = CFG_RIGHT;
	}
	else if (buttonActivated(B_LEFT)) {
		which_configuration = CFG_LEFT;
	}
	else if (buttonActivated(B_UP)) {
		which_configuration = CFG_UP;
	}
	else if (buttonActivated(B_LB)) {
		which_configuration = CFG_LB;
	}	
	else {
		which_configuration = global_header.default_configuration;
	}
	displayWhichConfiguration(which_configuration);	
	return which_configuration;
}



