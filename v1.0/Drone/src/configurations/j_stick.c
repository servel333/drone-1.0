/*
 * j_stick.c
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
#include <string.h>
#include "j_stick.h"
#include "debug.h"
#include "bluetooth.h"
#include "configurations.h"
#include "shared.h"
#include "system_time.h"

const static uint16_xy * center[2] = {(uint16_xy*)&(joysticks[JOYSTICK_LEFT].center_x),(uint16_xy*)&(joysticks[JOYSTICK_RIGHT].center_x)};

// Local structure for holding the joystick positions.
struct JStickJoystickXY {
	int8_t x;
	int8_t y;
};

static struct JStickJoystickXY current_xy[NUM_JOYSTICKS];
static struct JStickJoystickXY previous_xy[NUM_JOYSTICKS];

// Local pointer to the current controller configuration.
static struct ControllerConfiguration *my_cc;

// Local for holding the currently pressed buttons.
static uint32_t pressed_buttons = 0;
static uint32_t previous_pressed_buttons = 0;

static uint8_t dpad_buttons = DPadNoAction;
static uint8_t joystick_buttonsLOW = 0x0; // 0 - 7
static uint8_t joystick_buttonsHIGH = 0x0; // 8 - 15

jstick_rpt_t report;
jstick_rpt_t prev_report;

bool jdelta = false;

// Local function to handle keyboard and joystick button presses.
//void jStickReport(void);

void jStickInit(struct ControllerConfiguration *cc) {
	my_cc = cc;
	
	uint8_t rpt_init[JSTICK_RPT_BYTES] = {JSTICK_RPT_HEADER1,JSTICK_RPT_HEADER2,JSTICK_RPT_THROTTLE,0,0,0,0,0,0,0};
	memcpy( &(report), rpt_init, JSTICK_RPT_BYTES );
	prev_report = report;
	
	BluetoothJoystickMode();
}

void jStickButtonEvent(enum ButtonName name, enum ButtonState state)
{

};

bool jStickButtonChanged()
{
	// Send a report if the buttons have changed:
	if(
	prev_report.dpad != report.dpad ||
	prev_report.buttons1 != report.buttons1 ||
	prev_report.buttons2 != report.buttons2
	)
	{
		memcpy( &(prev_report.dpad), &(report.dpad), JSTICK_RPT_B_BYTES); // Update the previous values
		return true;
	}
	return false;
}

bool jStickJoystickChanged(void); //Prototype
bool jStickJoystickChanged()
{
	if(
	prev_report.leftx != report.leftx   ||
	prev_report.lefty  != report.lefty  ||
	prev_report.rightx != report.rightx ||
	prev_report.righty != report.righty
	)
	{
		memcpy(&(prev_report.leftx), &(report.leftx), JSTICK_RPT_J_BYTES); // Update the previous values
		return true;
	}
	return false;
}

void jStickButtonHandler(enum ButtonName name, enum ButtonState state) {
	
	//Move to ButtonChange()
	if (state == BUTTON_PRESSED) {
		// Set the corresponding bit:
		pressed_buttons |= 1ul<<name;
	}

	if (state == BUTTON_RELEASED) {
		// Clear the corresponding bit:
		pressed_buttons &= ~(1ul<<name);
	}
	
	if(name==REPORT)
	{
	
		BIT_SET(joystick_buttonsLOW, Button_A, BIT_GET(pressed_buttons, BIT(B_A)));
		BIT_SET(joystick_buttonsLOW, Button_B, BIT_GET(pressed_buttons, BIT(B_B)));
		BIT_SET(joystick_buttonsLOW, Button_X, BIT_GET(pressed_buttons, BIT(B_X)));
		BIT_SET(joystick_buttonsLOW, Button_Y, BIT_GET(pressed_buttons, BIT(B_Y)));

		BIT_SET(joystick_buttonsLOW, Button_LBumper, BIT_GET(pressed_buttons, BIT(B_LB)));
		BIT_SET(joystick_buttonsLOW, Button_RBumper, BIT_GET(pressed_buttons, BIT(B_RB)));
	
		BIT_SET(joystick_buttonsHIGH, Button_LTrigger, BIT_GET(pressed_buttons, BIT(B_LT)));
		BIT_SET(joystick_buttonsHIGH, Button_RTrigger, BIT_GET(pressed_buttons, BIT(B_RT)));
	
		BIT_SET(joystick_buttonsHIGH, Button_Select, BIT_GET(pressed_buttons, BIT(B_SELECT)));
		BIT_SET(joystick_buttonsHIGH, Button_Start, BIT_GET(pressed_buttons, BIT(B_START)));
	
		BIT_SET(joystick_buttonsHIGH, Button_LThumb, BIT_GET(pressed_buttons, BIT(B_JL)));
		BIT_SET(joystick_buttonsHIGH, Button_RThumb, BIT_GET(pressed_buttons, BIT(B_JR)));
	
		dpad_buttons = DPadNoAction;
	
		if (BIT_GET(pressed_buttons, BIT(B_UP)))
		{
			dpad_buttons = DPadUP;
		}
		else if (BIT_GET(pressed_buttons, BIT(B_LEFT)))
		{
			dpad_buttons = DPadLeft;
		}
		else if (BIT_GET(pressed_buttons, BIT(B_DOWN)))
		{
			dpad_buttons = DPadDown;
		}
		else if (BIT_GET(pressed_buttons, BIT(B_RIGHT)))
		{
			dpad_buttons = DPadRight;
	}
	
		report.dpad = dpad_buttons;
		report.buttons1 = joystick_buttonsLOW;
		report.buttons2 = joystick_buttonsHIGH;
		
		if( jStickButtonChanged() )
		{
			jdelta = true;
		}
	}
	
}

void jStickJoystickHandler(enum JoystickName name, int16_t x, int16_t y) {
	struct Joystick *this_joystick = &joysticks[name];
	JoystickConfiguration *this_joystick_cfg = &my_cc->joystick_configs[name];
	
	uint16_xy raw = {x,y};
	int8_xy out;
	AnalogJoystickAdj( center[name], this_joystick_cfg, &raw, &out );
	
	if( name==JOYSTICK_LEFT)
	{
		report.leftx = out.x;
		report.lefty = out.y;
	}
	else
	{
		//assert(name==JOYSTICK_RIGHT);
		report.rightx = out.x;
		report.righty = out.y;
	
		if(jStickJoystickChanged())
		{
			jdelta = true; //sets do_send_rpt if not already set
		}
	}
}

// Sends a joystick report when needed:
void jStickSendReport(void) {
	int i;
	unsigned char * u;
	
	u = (unsigned char *) &report;
	
	for(i=0; i<JSTICK_RPT_BYTES; i++)
	{
		sendBTChar(*u);
		u++;
	}
}