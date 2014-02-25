/*
 * gamepad.h
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

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "buttons.h"
#include "joysticks.h"

#define GAMEPAD_RESOLUTION 1

//(val<<pos)  The Value to be inserted into the Position
#define DPadUP (0x00<<0)
#define DPadUPRight (0x01<<0)
#define DPadRight (0x02<<0)
#define DPadDownRight (0x03<<0)
#define DPadDown (0x04<<0)
#define DPadDownLeft (0x05<<0)
#define DPadLeft (0x06<<0)
#define DPadUpLeft (0x07<<0)
#define DPadNoAction (0x08<<0)

#define Button_A (0x01<<0)
#define Button_B (0x01<<1)
#define Button_C (0x01<<2) // Not Used?
#define Button_X (0x01<<3)
#define Button_Y (0x01<<4)
#define Button_Z (0x01<<5)
#define Button_LBumper (0x01<<6)
#define Button_RBumper (0x01<<7)

#define Button_LTrigger (0x01<<0)
#define Button_RTrigger (0x01<<1)
#define Button_Select (0x01<<2)
#define Button_Start (0x01<<3)
#define Button_Mode (0x01<<4) // Not Used or Select
#define Button_LThumb (0x01<<5)
#define Button_RThumb (0x01<<6)
#define Button_Unknown (0x01<<7)

void gamepadInit(struct ControllerConfiguration *cc);
void gamepadButtonHandler(enum ButtonName name, enum ButtonState state);
void gamepadJoystickHandler(enum JoystickName name, int16_t x, int16_t y);

void GamepadButtonEvent(enum ButtonName name, enum ButtonState state);
bool GamepadButtonChanged(void);

extern bool delta; //buttons or joysticks changed?

void GamepadSendReport(void);

/*
* Gamepad Report Format
*
* Identifier (A1,04)
* Throttle (not used)
* X1
* Y1
* X2
* Y2
* HatSwitch / DPAD
* Buttons 7-0
* Buttons 15-8
*/
typedef struct
{
	uint8_t header1; //Initialized with 0xA1
	uint8_t header2; //Initialized with 0x04
	int8_t throttle; //Not used; Initialized with 0x00
	int8_t leftx;    //Left joystick x-axis; -127 to +127
	int8_t lefty;	//Left joystick y-axis; -127 to +127
	int8_t rightx;	//Right joystick x-axis; -127 to +127
	int8_t righty;	//Right joystick y-axis; -127 to +127
	uint8_t dpad;	//Dpad
	uint8_t buttons1;
	uint8_t buttons2;	
} gamepad_rpt_t;

#define GAMEPAD_RPT_BYTES 10
#define GAMEPAD_RPT_J_BYTES 4
#define GAMEPAD_RPT_B_BYTES 3

#define GAMEPAD_RPT_HEADER1 0xA1
#define GAMEPAD_RPT_HEADER2 0x04
#define GAMEPAD_RPT_THROTTLE 0

#endif /* GAMEPAD_H_ */