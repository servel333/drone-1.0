/**
 * init.c
 * Version: Drone Open Beta 1.0
 *
 * Copyright (C) 2013 EVO CONTROLLERS, LLC.
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

#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	// Enable ioport:
	ioport_init();

	// Setup the power pins:
	ioport_set_pin_level(POWER_SWITCH_PIN, 1); // 1- enable power, 0 - disable power
	ioport_set_pin_dir  (POWER_SWITCH_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (POWER_SWITCH_PIN, IOPORT_MODE_WIREDOR);

	ioport_set_pin_dir (CHRG_STAT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(CHRG_STAT_PIN, IOPORT_MODE_PULLUP);// IOPORT_MODE_TOTEM);

	ioport_set_pin_dir (BATTERY_DETECT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(BATTERY_DETECT_PIN, IOPORT_MODE_TOTEM);
	
	// Setup the buttons:
	ioport_set_pin_dir (UP_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(UP_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (DOWN_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(DOWN_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (LEFT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(LEFT_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (RIGHT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(RIGHT_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (A_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(A_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (B_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(B_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (X_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(X_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (Y_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(Y_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (START_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(START_PIN, IOPORT_MODE_PULLDOWN); // IOPORT_MODE_INVERT_PIN is an option to remove polarity bit from struct Button.

	ioport_set_pin_dir (SELECT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(SELECT_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (RB_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(RB_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (LB_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(LB_PIN, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir (RT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(RT_PIN, IOPORT_MODE_PULLDOWN);

	ioport_set_pin_dir (LT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(LT_PIN, IOPORT_MODE_PULLDOWN);
	
	// Setup the joysticks:
	ioport_set_pin_dir (JOYSTICK_L_X_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(JOYSTICK_L_X_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_dir (JOYSTICK_L_Y_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(JOYSTICK_L_Y_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_dir (JOYSTICK_R_X_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(JOYSTICK_R_X_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_dir (JOYSTICK_R_Y_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(JOYSTICK_R_Y_PIN, IOPORT_MODE_TOTEM);
	
	// Setup the bluetooth pins:	
	ioport_set_pin_level(BT_RESET_PIN, true); // Hold it in reset.
	ioport_set_pin_dir  (BT_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BT_RESET_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(BT_FACTORY_RESET_PIN, true); // Float to default (low)
	ioport_set_pin_dir  (BT_FACTORY_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BT_FACTORY_RESET_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(BT_CTS_PIN, false); // Default - enable transmitter.
	ioport_set_pin_dir  (BT_CTS_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode (BT_CTS_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_dir  (BT_RTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BT_RTS_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(BT_TX_PIN, true); // Hold it high.
	ioport_set_pin_dir  (BT_TX_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BT_TX_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(BT_RX_PIN, true);
	ioport_set_pin_dir  (BT_RX_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode (BT_RX_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(BT_ENABLE_PIN, false); // Hold power off (low)
	ioport_set_pin_dir  (BT_ENABLE_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BT_ENABLE_PIN, IOPORT_MODE_TOTEM);

	// Setup led pins: (Default: off)
	ioport_set_pin_level(RED_LED_PIN, true); 
	ioport_set_pin_dir  (RED_LED_PIN,   IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (RED_LED_PIN,   IOPORT_MODE_TOTEM);
	
	ioport_set_pin_level(GREEN_LED_PIN, true);
	ioport_set_pin_dir  (GREEN_LED_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (GREEN_LED_PIN, IOPORT_MODE_TOTEM);
	
	ioport_set_pin_level(BLUE_LED_PIN, true);
	ioport_set_pin_dir  (BLUE_LED_PIN,  IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (BLUE_LED_PIN,  IOPORT_MODE_TOTEM);

	// Setup USB UART:
	ioport_set_pin_level(USB_TX_PIN, true); // Hold it high.
	ioport_set_pin_dir  (USB_TX_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (USB_TX_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(USB_RX_PIN, true);
	ioport_set_pin_dir  (USB_RX_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode (USB_RX_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_level(USB_RESET_PIN, true); // Release it from reset.
	ioport_set_pin_dir  (USB_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (USB_RESET_PIN, IOPORT_MODE_TOTEM);

	// Setup debug UART:
	ioport_set_pin_level(DEBUG_TX_PIN, true); // Hold it high.
	ioport_set_pin_dir  (DEBUG_TX_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode (DEBUG_TX_PIN, IOPORT_MODE_TOTEM);

	ioport_set_pin_dir  (DEBUG_RX_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode (DEBUG_RX_PIN, IOPORT_MODE_TOTEM);
	
	// Manufacture Test Pins
	ioport_set_pin_dir(IsManufactureTestMode, IOPORT_DIR_INPUT); // Input from Test Jig
	ioport_set_pin_mode(IsManufactureTestMode, IOPORT_MODE_PULLDOWN);	
	
	ioport_set_pin_dir(DRONE_GPIO_0, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(DRONE_GPIO_0, IOPORT_MODE_PULLDOWN);
	
	ioport_set_pin_dir(DRONE_GPIO_1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(DRONE_GPIO_1, IOPORT_MODE_PULLDOWN);
	
	ioport_set_pin_dir(DRONE_GPIO_2, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(DRONE_GPIO_2, IOPORT_MODE_PULLDOWN);
	
	ioport_set_pin_dir(DRONE_GPIO_3, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(DRONE_GPIO_3, IOPORT_MODE_PULLDOWN);
}
