/*
 * joysticks.c
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

#include "joysticks.h"
#include "shared.h"
#include "configurations.h"

struct Joystick joysticks[NUM_JOYSTICKS] = {
	{JOYSTICK_ADC_L_X, JOYSTICK_ADC_L_Y, 0, 0, 0, 0, 0, 0},
	{JOYSTICK_ADC_R_X, JOYSTICK_ADC_R_Y, 0, 0, 0, 0, 0, 0}
};




//Center calibration, Unsigned to Signed conversion, and axis flipping
//compare AnalogJoystickAdj
void SimpleJoystickAdj( const uint16_xy * center, const int16_xy * deadzone, const uint16_xy * raw, int16_xy * out )
{
	int16_t x_tmp = raw->x;
	int16_t y_tmp = raw->y;
	
	// Correct for the center position:
	x_tmp -= center->x; //calibration and unsigned to signed conversion
	y_tmp -= center->y; //calibration and unsigned to signed conversion
	
	//gamepadJoystickHandler() flips axis with min/max pairing for scaling
	//we use simple flipping here:
	x_tmp = -x_tmp;
	y_tmp = -y_tmp;
	
	//This deadzone code looks at each axis independently - digital behavior joysticks
	//AnalogJoystickAdj looks at both axises together - analog behavior joysticks
	if(NABS(x_tmp) > -(deadzone->x))
	{
		x_tmp=0;
	}
			
	if(NABS(y_tmp) > -(deadzone->y))
	{
		y_tmp=0;
	}	
	
	out->x = x_tmp;
	out->y = y_tmp;
}


//map from ADC range [0,4095] to joystick range [-127,+127]
//Joystick orientations:
//left:- , right:+
//up:- , down:+
void AnalogJoystickAdj( const uint16_xy * center, JoystickConfiguration *this_joystick_cfg, const uint16_xy * raw, int8_xy * out )
{
	int16_t x_tmp = raw->x;
	int16_t y_tmp = raw->y;
	
	/*
	//Implementation #1
	int16_t nabs;
	
	nabs = NABS(x);
	if( nabs > -(this_joystick_cfg->x_deadzone) )
	{
		x = 0;
	}
	else
	{
		x = map(x, this_joystick_cfg->x_min, this_joystick_cfg->x_max, GAMEPAD_X_MIN, GAMEPAD_X_MAX );
	}
	nabs = NABS(y);
	if( nabs > -(this_joystick_cfg->y_deadzone) )
	{
		y = 0;
	}
	else
	{
		y = map(y, this_joystick_cfg->y_min, this_joystick_cfg->y_max, GAMEPAD_Y_MIN, GAMEPAD_Y_MAX );
	}
	*/	


	/*
	//Implementation #2
	// Correct for the center position:
	x -= this_joystick->center_x; //calibration and unsigned to signed conversion
	y -= this_joystick->center_y; //calibration and unsigned to signed conversion
	
	x = x>>4; //resolution reduction from 12b to 8b
	y = y>>4; //resolution reduction from 12b to 8b
	
	x=-x; //flip x //left:- , right:+
	y=-y; //flip y //up:- , down:+
	
	if( (x*x + y*y) < DEADZONE_SQUARED )
	{
		x = 0;
		y = 0;
	}
	else
	{
		//x = x>0 ? 1 : -1;
		//y = y>0 ? 1 : -1;
	}
	*/	
	
	//Implementation #3
	// Correct for the center position:
	x_tmp -= center->x; //calibration and unsigned to signed conversion
	y_tmp -= center->y; //calibration and unsigned to signed conversion
	
	//#if DEBUG
	//if(name==1 )
	//{
		////PrintXY(joysticks[i].current_x,joysticks[i].current_y); //raw ADC values
		//PrintXY(x,y);
		//sendUSBString("->", false);
	//}
	//#endif
	
	
	/*
	//magnitude method deadzone
	mag_squared = (x*x + y*y);
	if( mag_squared < deadzone_squared )
	{
		x = 0;
		y = 0;
	}
	*/
	
	//slightly better, simple method deadzone (deadzone box at the center) 
	//regular simple method (check x separate from y) creates deadzone cross on x-axis and y-axis 
	if( 
		(NABS(x_tmp) > -(this_joystick_cfg->x_deadzone)) && 
		(NABS(y_tmp) > -(this_joystick_cfg->y_deadzone))
	)
	{
		x_tmp=0;
		y_tmp=0;
	}
	else
	{
		if(x_tmp>0)
		{
			x_tmp = ( (int32_t)GAMEPAD_X_MIN*x_tmp ) / (this_joystick_cfg->x_max); //min/max is (-)/(+)
		}
		else
		{
			x_tmp = ( (int32_t)GAMEPAD_X_MAX*x_tmp ) / (this_joystick_cfg->x_min); //max/min is (+)/(-)
		}
		if(y_tmp>0)
		{
			y_tmp = ( (int32_t)GAMEPAD_Y_MIN*y_tmp ) / (this_joystick_cfg->y_max); //min/max is (-)/(+)
		}
		else
		{
			y_tmp = ( (int32_t)GAMEPAD_Y_MAX*y_tmp ) / (this_joystick_cfg->y_min); //max/min is (+)/(-)
		}
	}
	
	//#if DEBUG
	//if(name==1)
	//{
		////PrintXY(debug_x,debug_y); //post-handler (mapped values)
		//PrintXY(x,y);
		//sendUSBLine("");
	//}
	//#endif
	
	
	
	// Adjust for sensitivity:
	//x /= GAMEPAD_RESOLUTION;
	//y /= GAMEPAD_RESOLUTION;
	
	// Gamepad range is [-127,127]. Fix if out of bounds:
	if (x_tmp > GAMEPAD_X_MAX) {
		x_tmp = GAMEPAD_X_MAX;
	}
	else if (x_tmp < GAMEPAD_X_MIN) {
		x_tmp = GAMEPAD_X_MIN;
	}
	
	if (y_tmp > GAMEPAD_Y_MAX) {
		y_tmp = GAMEPAD_Y_MAX;
	}
	else if (y_tmp < GAMEPAD_Y_MIN) {
		y_tmp = GAMEPAD_Y_MIN;
	}	
	
	out->x = (int8_t)x_tmp;
	out->y = (int8_t)y_tmp;
}




//map from ADC range [0,4095] to mouse range [-2048,+2047]
//Joystick orientations:
//left:- , right:+
//up:- , down:+
void MouseJoystickAdj( const uint16_xy * center, JoystickConfiguration *this_joystick_cfg, const uint16_xy * raw, int16_xy * out )
{
	int16_t x_tmp = raw->x;
	int16_t y_tmp = raw->y;
	
	//Implementation #3
	// Correct for the center position:
	x_tmp -= center->x; //calibration and unsigned to signed conversion
	y_tmp -= center->y; //calibration and unsigned to signed conversion
	
	//slightly better, simple method deadzone (deadzone box at the center) 
	//regular simple method (check x separate from y) creates deadzone cross on x-axis and y-axis 
	if( 
		(NABS(x_tmp) > -(this_joystick_cfg->x_deadzone)) && 
		(NABS(y_tmp) > -(this_joystick_cfg->y_deadzone))
	)
	{
		x_tmp=0;
		y_tmp=0;
	}
	else
	{
		
		/*
		if(x_tmp>0)
		{
			x_tmp = ( (int32_t)MOUSE_X_MIN*x_tmp ) / (this_joystick_cfg->x_max); //min/max is (-)/(+)
		}
		else
		{
			x_tmp = ( (int32_t)MOUSE_X_MAX*x_tmp ) / (this_joystick_cfg->x_min); //max/min is (+)/(-)
		}
		if(y_tmp>0)
		{
			y_tmp = ( (int32_t)MOUSE_Y_MIN*y_tmp ) / (this_joystick_cfg->y_max); //min/max is (-)/(+)
		}
		else
		{
			y_tmp = ( (int32_t)MOUSE_Y_MAX*y_tmp ) / (this_joystick_cfg->y_min); //max/min is (+)/(-)
		}
		*/
		x_tmp = -x_tmp;
		y_tmp = -y_tmp;
		
		//x_tmp /= MOUSE_RESOLUTION;
		//y_tmp /= MOUSE_RESOLUTION;
	}
	
	// Adjust for sensitivity:
	//x /= MOUSE_RESOLUTION;
	//y /= MOUSE_RESOLUTION;
	
	// Gamepad range is [-2048,2047]. Fix if out of bounds:
	if (x_tmp > MOUSE_X_MAX) {
		x_tmp = MOUSE_X_MAX;
	}
	else if (x_tmp < MOUSE_X_MIN) {
		x_tmp = MOUSE_X_MIN;
	}
	
	if (y_tmp > MOUSE_Y_MAX) {
		y_tmp = MOUSE_Y_MAX;
	}
	else if (y_tmp < MOUSE_Y_MIN) {
		y_tmp = MOUSE_Y_MIN;
	}	
	
	out->x = (int16_t)x_tmp;
	out->y = (int16_t)y_tmp;
}


