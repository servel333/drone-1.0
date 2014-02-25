/*
 * bluetooth.h
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

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include <compiler.h>

unsigned char recvBTChar(void);

void setupBluetoothFast(void);
void sendBTChar(unsigned char str);
void sendBTHexPair(unsigned char binaryData);
void sendBTString(const char *string);
void sendBTLine(const char *string);
void turnBTOn(void);
void turnBTOff(void);
void initBTComms(const char * modeName);
void applyBTSettings(void);
void btFactoryReset(void);
void btClearAutoConnectList(void);
void bt_exitBypass(void);
void bt_homeButton(void);

bool sendBTCmd(const char *string);
bool QueryDiscoverable(void);
bool QueryConnected(void);
bool hardwareflowcontrol(int maxWaitTime); // maxWaitTime in ms
bool btIsConnected(void);
bool BluetoothKeyboardMode(void);
bool BluetoothMouseMode(void);
bool BluetoothComboMode(void);
bool BluetoothGamePadMode(void);
bool BluetoothJoystickMode(void);

#endif /* BLUETOOTH_H_ */