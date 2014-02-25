// $Id$

#ifndef JOYSTICK_TYPES_H_
#define JOYSTICK_TYPES_H_

struct Joystick {
	enum adcch_positive_input adc_channel_x, adc_channel_y;
	uint16_t center_x, center_y;
	uint16_t previous_x, previous_y;
	uint16_t current_x, current_y;
};

enum JoystickName {
	JOYSTICK_LEFT = 0,
	JOYSTICK_RIGHT,
	NUM_JOYSTICKS
};

typedef void (*JoystickHandler)(enum JoystickName, int16_t /*x*/, int16_t /*y*/);

#endif /* JOYSTICK_TYPES_H_ */