/*
 * comm.c
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

#include "comm.h"
#include "usb.h"
#include "configurations.h"
#include <string.h>
#include "debug.h"
#include "shared.h"
#include "bluetooth.h"

// The maximum amount of data that can be transmitted at a time.
#define WG_MAX_LENGTH 8
#define WC_MAX_LENGTH 8

// ========
// Declaration of command functions:
// ========
// Read the global header and load it into the working set. (resets AG)
// Example:
// > RG
// < 1234567890ABCDEF11223344556677889900AABBCCDDEEFF
// < OK
void commRG(char* args);

// Set the address to write to on the working copy of the global header.
// Example: Set the address to 44
// > AG2C
// < OK
void commAG(char* args);

// Write to the working copy of the global header and increment the address.
// Note that the line (including terminator) must not be longer than USB_SERIAL_RX_BUFFER_SIZE.
// Example: Write 8 bytes to the working global header copy
// > WG1122334455
// < OK
// > WG667788
// < OK
void commWG(char* args);

// Save the working copy of the global header to memory. (resets AG)
// Example:
// > SG
// < OK
void commSG(char* args);

// Read the selected controller configuration and load it into the working set. (resets AC)
// Example: Read the second controller configuration
// > RC02
// < 12345678901234567890ABCDEFABCDEF
// < OK
void commRC(char* args);

// Set the address to write to on the working copy of the controller configuration.
// Example: Set the address to 44
// > AC2C
// < OK
void commAC(char* args);

// Write to the working copy of the controller configuration and increment the address.
// Note that the line (including terminator) must not be longer than USB_SERIAL_RX_BUFFER_SIZE.
// Example: Write 8 bytes to the working controller configuration copy
// > WC1122334455
// < OK
// > WC667788
// < OK
void commWC(char* args);

// Save the working copy of the controller configuration to memory. (resets AC)
// Example:
// > SC
// < OK
void commSC(char* args);

// Do an Internal Reset
void commRS(char* args);

// Jump to the Bootloader
void commBL(char* args);

// Command to update bluetooth
void commBU(char* args);

// Sends command response.
void sendCommOK(void);
void sendCommNO(void);

// Commands that can be sent to the device:
// All commands are terminated with a '\0' and will respond with "OK" or "NO".
#define NUM_COMMANDS 11
const struct Command COMMANDS[] = {
	{"RG", commRG},
	{"AG", commAG},
	{"WG", commWG},
	{"SG", commSG},
	{"RC", commRC},
	{"AC", commAC},
	{"WC", commWC},
	{"SC", commSC},
	{"RS", commRS},
	{"BL", commBL},
	{"BU", commBU},
};

// Local structures for holding eeprom data:
static struct GlobalHeader ghTemp;
static struct ControllerConfiguration ccTemp;

// Local "pointers" for the working sets:
static uint8_t ghAddr;
static uint8_t ccAddr;

void byteToASCIIHex(const uint8_t b, char* high, char* low) {
	*low = b & 0x0F;
	*high = (b>>4) & 0x0F;
	
	if (*low > 9) {
		*low = 'a' + *low - 0x0A;
	}
	else {
		*low = '0' + *low;
	}

	if (*high > 9) {
		*high = 'a' + *high - 0x0A;
	}
	else {
		*high = '0' + *high;
	}
}

uint8_t ASCIIHexToByte(char high, char low) {
	uint8_t temp;
	
	// Make high and low lower case:
	high |= 0x20;
	low  |= 0x20;
	
	if (high >= 'a') {
		temp = 0x0A + high - 'a';
	}
	else {
		temp = high - '0';
	}
	
	temp <<= 4;
	
	if (low >= 'a') {
		temp += 0x0A + low - 'a';
	}
	else {
		temp += low - '0';
	}
	
	return temp;
}

void processComm(void) {
	uint8_t i;
	char str[USB_SERIAL_RX_BUFFER_SIZE];
	char cmd[3];
	bool match = false;
	
	// Was a string transmitted?
	if (recvUSBString(str, USB_SERIAL_RX_BUFFER_SIZE)) {
		cmd[0] = str[0];
		cmd[1] = str[1];
		cmd[2] = '\0';
		sendDBGLine("USB:rcv" + *cmd);
		// Search for the command:
		for (i = 0; i < NUM_COMMANDS; i++) {
			// Is this a match?
			if (0 == strcmp(COMMANDS[i].CMD, cmd)) {
				sendDBGString("Command match ");
				sendDBGLine(COMMANDS[i].CMD);
				// Then execute the command passing it the rest of the string:
				match = true;
				COMMANDS[i].action(str+2);
				break;
			}
		}
		
		// If the command was not found, Then send a NO:
		if (!match) {
			sendDBGString("No command matching ");
			sendDBGLine(cmd);
			sendCommNO();
		}
	}
}

void sendCommOK(void) {
	sendUSBString("OK", true);
}

void sendCommNO(void) {
	sendUSBString("NO", true);
}

void commRG(char* args) {
	uint8_t i;
	char high, low;
	
	// Read the global header:
	readGlobalHeader(&ghTemp);
	
	// Write each ASCII byte:
	for (i = 0; i < sizeof(struct GlobalHeader); i++) {
		byteToASCIIHex(((uint8_t*)&ghTemp)[i], &high, &low);
		sendUSBChar(high);
		sendUSBChar(low);
	}
	
	// Add the terminator:
	sendUSBChar('\0');
	
	// Send an OK:
	sendCommOK();
	
	// Reset the working copy address:
	ghAddr = 0;
}

void commAG(char* args) {
	// Check for the argument:
	if (args[0] == '\0' || args[1] == '\0' || args[2] != '\0') {
		sendCommNO();
	}
	else {
		// Set the new address:
		ghAddr = ASCIIHexToByte(args[0], args[1]);
		
		// Send an OK:
		sendCommOK();
	}
}

void commWG(char* args) {
	uint8_t length;
	uint8_t temp = 0;
	
	// How much data is being written?
	length = strlen(args);
	
	// Length must be an even number since it represents ASCII coded hex.
	if ((length & 1) == 1) {
		sendCommNO();
		return;
	}
	
	// Translate the ASCII to hex and write it to the working copy as long as there is room:
	while (temp < length && ghAddr < sizeof(struct GlobalHeader)) {
		((uint8_t*)&ghTemp)[ghAddr++] = ASCIIHexToByte(args[temp], args[temp+1]);
		temp += 2;
	}
	
	// Did the copy complete?
	if (temp == length) {
		sendCommOK();
	}
	else {
		sendCommNO();
	}
}

void commSG(char* args) {
	uint8_t i;
	char high, low;
	
	// Read the global header:
	writeGlobalHeader(&ghTemp);
	
	// Send an OK:
	sendCommOK();
	
	// Reset the working copy address:
	ghAddr = 0;
}

void commRC(char* args) {
	uint8_t i, which;
	char high, low;
	
	// Check for the argument and terminator:
	if (args[0] == '\0' || args[1] == '\0' || args[2] != '\0') {
		sendCommNO();
		return;
	}
	
	// Get the argument:
	which = ASCIIHexToByte(args[0], args[1]);
	
	// Read the controller configuration:
	readControllerConfig(which, &ccTemp);
	
	// Write each ASCII byte:
	for (i = 0; i < sizeof(struct ControllerConfiguration); i++) {
		byteToASCIIHex(((uint8_t*)&ccTemp)[i], &high, &low);
		sendUSBChar(high);
		sendUSBChar(low);
	}
	
	// Add the terminator:
	sendUSBChar('\0');
	
	// Send an OK:
	sendCommOK();
	
	// Reset the working copy address:
	ccAddr = 0;
}

void commAC(char* args) {
	// Check for the argument:
	if (args[0] == '\0' || args[1] == '\0' || args[2] != '\0') {
		sendCommNO();
	}
	else {
		// Set the new address:
		ccAddr = ASCIIHexToByte(args[0], args[1]);
	
		// Send an OK:
		sendCommOK();
	}
}

void commWC(char* args) {
	uint8_t length;
	uint8_t temp = 0;
	
	// How much data is being written?
	length = strlen(args);
	
	// Length must be an even number since it represents ASCII coded hex.
	if ((length & 1) == 1) {
		sendCommNO();
		return;
	}
	
	// Translate the ASCII to hex and write it to the working copy as long as there is room:
	while (temp < length && ccAddr < sizeof(struct ControllerConfiguration)) {
		((uint8_t*)&ccTemp)[ccAddr++] = ASCIIHexToByte(args[temp], args[temp+1]);
		temp += 2;
	}
	
	// Did the copy complete?
	if (temp == length) {
		sendCommOK();
	}
	else {
		sendCommNO();
	}
}

void commSC(char* args) {
	uint8_t which;
	
	// Check for the argument and terminator:
	if (args[0] == '\0' || args[1] == '\0' || args[2] != '\0') {
		sendCommNO();
		return;
	}
	
	// Get the argument:
	which = ASCIIHexToByte(args[0], args[1]);
	
	// Write the controller configuration:
	writeControllerConfig(which, &ccTemp);

	// Send an OK:
	sendCommOK();

	// Reset the working copy address:
	ccAddr = 0;
}

void commRS(char* args)
{	
	// Send an OK:
	sendCommOK();
	delay_ms(10);
	reset_do_soft_reset();
}

void commBL(char* args)
{
	sendCommOK();
	delay_ms(10);
	bootloader();
}

void commBU(char* args)
{	 
	
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true);
	delay_ms(1);
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, false);
	delay_ms(1);
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true);
	delay_ms(1);
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, false);
	delay_ms(1);
	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true);
	delay_ms(1);
	sendBTCmd("BC:FU");
}