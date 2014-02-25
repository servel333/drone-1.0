/*
 * battery_charger.h
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

#ifndef BATTERY_CHARGER_H_
#define BATTERY_CHARGER_H_

typedef enum {	
	CHARGING = 0,
	CHARGER_ERROR = 1,
	NOT_CHARGING = 3 // 3 Because of the Hi-Z method	
} ChargeState_t;

ChargeState_t getChargeState(void);
void displayChargeState(void);

void MaintainControllerStatus(bool pulse, bool chargingDone, uint16_t battery_level);

bool IsChargingDone(void);

void SetupChrgStatPinExternalInterrupt(void);

#endif /* BATTERY_CHARGER_H_ */