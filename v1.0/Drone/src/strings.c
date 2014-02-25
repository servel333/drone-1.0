/*
 * strings.c
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

#include "strings.h"

// usage:
// pgm_read_byte(rn)
// General 
const char rn[]					PROGMEM = "\r\n";
const char good[]				PROGMEM = "GOOD";
const char bad[]				PROGMEM = "BAD";

// Debug Strings
const char debuguartinit[]		PROGMEM = "DEBUG UART INIT: ";
const char debug[]				PROGMEM = "DEBUG ";
const char btFlowControlWorks[]	PROGMEM = "Bluetooth: FlowControl Works.";
const char btFlowControlFail[]	PROGMEM = "Bluetooth: FlowControl FAIL";
const char btIOTest[]			PROGMEM = "Bluetooth: IO Test";

// Bluetooth Commands
const char btCancelBypass[]		PROGMEM = "BC:BP=00,00";
const char btBypassMode[]		PROGMEM = "BC:BP=04,07";
const char btKeyboardMode[]		PROGMEM = "BC:CD=000540";
const char btMouseMode[]		PROGMEM = "BC:CD=000580";
const char btComboMode[]		PROGMEM = "BC:CD=0005C0";
const char btGamePadMode[]		PROGMEM = "BC:CD=0005C8";
const char btJoystickMode[]		PROGMEM = "BC:CD=0005C4";
const char btConnectHID[]		PROGMEM = "BC:CI";
const char btProxyMode[]		PROGMEM = "BC:DB=00,00";
const char btBinaryMode[]		PROGMEM = "BC:DB=04,07";
const char btDisconnectHID[]	PROGMEM = "BC:DI";
const char btClearPairedList[]	PROGMEM = "BC:DC";
const char btConnConfig1[]		PROGMEM = "BC:FT=FF,00,FF,0A";
const char btConnConfig2[]		PROGMEM = "BC:FT=00,00,FF,0A";
const char dfumode[]			PROGMEM = "BC:FU";
const char btHIDInquiry[]		PROGMEM = "BC:IS";
const char keyboardRptPrefix[]	PROGMEM = "BC:KR=A1,01,00,";
const char mouseRptPrefix[]		PROGMEM = "BC:KR=A1,03,";
const char gamepadRptPrefix[]	PROGMEM = "BC:KR=A1,04,";
const char btDiscoveryEnable[]	PROGMEM = "BC:MD=01";
const char btDiscoveryMode[]	PROGMEM = "BC:MD";
const char btConfigNameCmd[]	PROGMEM = "BC:NM=";
const char btIO8High[]			PROGMEM = "BC:PC=08,01";
const char btIO9Low[]			PROGMEM = "BC:PC=09,00";
const char btProfilesCmd[]		PROGMEM = "BC:PF=00,01,00";
const char btManufactureNameCmd[]	PROGMEM = "BC:SM=";
const char btSerialInquiry[]	PROGMEM = "BC:SS";
const char btManufacturer[]		PROGMEM = "Drone";

// Manufacture Messages
const char unsupportedTest[]	PROGMEM = "Unsupported Test";
const char notImplementedTest[]	PROGMEM = "Not Implemented Test";
const char testText[]			PROGMEM = "%cTest%b: %c";
const char resultsExternal[]	PROGMEM = "Results External";
const char testMode[]			PROGMEM = "Drone in Test Mode";
const char testModeDirections[]	PROGMEM = "Toggle GPIO pins for actual Tests";
const char voltageTests[]		PROGMEM = "Voltage Tests";
const char ledTest[]			PROGMEM = "LED Test";
const char usbTX[]				PROGMEM = "USB Transmit";
const char usbTXText[]			PROGMEM = "USB Transmit Test";

const char chargeStatus[]		PROGMEM = "Charge Status";

const char debugTurnBTOn[]		PROGMEM = "turnBTOn";
const char debugTurnBTOff[]		PROGMEM = "turnBTOff";
const char debugDiscovery[]		PROGMEM = "Discovery";
const char debugApplyBTSetting[]	PROGMEM = "applyBTSettings";
const char debugInitBTComms[]	PROGMEM = "initBTComms";

const char comma[]				PROGMEM = ",";
const char zero[]				PROGMEM = "0";
// Test was 2 lines of code 4 + 3 ASCII + Null Terminator = 9 bytes
// Memory usage results were the same for both PSTR and pgm_read_byte
// The pgm_read_byte is a little less readable, but is a nice way to centralize const strings.
// First usage in a class may increase Program Memory Usage but the Data Memory is more important.

//sendDBGLine("GOOD");
//sendDBGLine("BAD");
// Program Memory Usage 	:	16898 bytes   24.3 % Full
// Data Memory Usage		:	2241 bytes   54.7 % Full

//sendDBGLine(PSTR("GOOD"));
//sendDBGLine(PSTR("BAD"));
// Program Memory Usage 	:	16898 bytes   24.3 % Full
// Data Memory Usage 		:	2233 bytes   54.5 % Full

//sendDBGLine(pgm_read_byte(good));
//sendDBGLine(pgm_read_byte(bad));
// Program Memory Usage 	:	16898 bytes   24.3 % Full
// Data Memory Usage		:	2233 bytes   54.5 % Full