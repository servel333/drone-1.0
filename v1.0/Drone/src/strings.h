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
#ifndef STRINGLITERALS_H_
#define STRINGLITERALS_H_

#include <avr\pgmspace.h>

extern const char rn[];
extern const char good[];
extern const char bad[];

extern const char debuguartinit[];
extern const char uartinit[];
extern const char debug[];

extern const char unsupportedTest[];
extern const char notImplementedTest[];

extern const char testText[];
extern const char resultsExternal[];

extern const char testMode[];
extern const char testModeDirections[];
extern const char voltageTests[];
extern const char ledTest[];
extern const char usbTX[];
extern const char usbTXText[];
extern const char chargeStatus[];

extern const char btDisconnectHID[];
extern const char btConnectHID[];
extern const char btClearPairedList[];

extern const char btFlowControlWorks[];
extern const char btFlowControlFail[];
extern const char btIOTest[];
extern const char btIO8High[];
extern const char btIO9Low[];
extern const char btProfilesCmd[];
extern const char btCancelBypass[];
extern const char btBypassMode[];
extern const char btProxyMode[];
extern const char btKeyboardMode[];
extern const char btMouseMode[];
extern const char btComboMode[];
extern const char btGamePadMode[];
extern const char btJoystickMode[];
extern const char btConnConfig1[];
extern const char btConnConfig2[];

extern const char btDiscoveryEnable[];
extern const char btDiscoveryMode[];
extern const char btSerialInquiry[];
extern const char btHIDInquiry[];

extern const char btManufactureNameCmd[];
extern const char btConfigNameCmd[];
extern const char btManufacturer[];
extern const char keyboardRptPrefix[];
extern const char mouseRptPrefix[];
extern const char gamepadRptPrefix[];

extern const char dfumode[];

extern const char debugTurnBTOn[];
extern const char debugTurnBTOff[];
extern const char debugDiscovery[];
extern const char debugApplyBTSetting[];
extern const char debugInitBTComms[];
extern const char comma[];
extern const char zero[];
#endif // STRINGLITERALS_H_

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