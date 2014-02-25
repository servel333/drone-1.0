/**
 * user_board.h
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

#ifndef USER_BOARD_H
#define USER_BOARD_H

/* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#include "ioport.h"

#define UP_PIN     IOPORT_CREATE_PIN(PORTB, 1)
#define DOWN_PIN   IOPORT_CREATE_PIN(PORTB, 5)
#define LEFT_PIN   IOPORT_CREATE_PIN(PORTB, 3)
#define RIGHT_PIN  IOPORT_CREATE_PIN(PORTA, 1)
#define A_PIN      IOPORT_CREATE_PIN(PORTA, 5)
#define B_PIN      IOPORT_CREATE_PIN(PORTB, 2)
#define X_PIN      IOPORT_CREATE_PIN(PORTA, 4)
#define Y_PIN      IOPORT_CREATE_PIN(PORTA, 7)
#define START_PIN  IOPORT_CREATE_PIN(PORTF, 2)
#define SELECT_PIN IOPORT_CREATE_PIN(PORTF, 5)
#define RB_PIN     IOPORT_CREATE_PIN(PORTF, 7)
#define LB_PIN     IOPORT_CREATE_PIN(PORTF, 6)
#define LT_PIN	   IOPORT_CREATE_PIN(PORTB, 4)
#define RT_PIN     IOPORT_CREATE_PIN(PORTA, 2)

// Joystick definitions:
#define JOYSTICK_L_X_PIN IOPORT_CREATE_PIN(PORTB, 0)
#define JOYSTICK_L_Y_PIN IOPORT_CREATE_PIN(PORTA, 6)
#define JOYSTICK_R_X_PIN IOPORT_CREATE_PIN(PORTB, 7)
#define JOYSTICK_R_Y_PIN IOPORT_CREATE_PIN(PORTB, 6)

#define JOYSTICK_ADC_L_X ADCCH_POS_PIN8
#define JOYSTICK_ADC_L_Y ADCCH_POS_PIN6
#define JOYSTICK_ADC_R_X ADCCH_POS_PIN15
#define JOYSTICK_ADC_R_Y ADCCH_POS_PIN14

#define JOYSTICK_ADC_MODULE	 ADCA
#define JOYSTICK_ADC_CHANNEL ADC_CH0

// LED definitions:
#define RED_LED_PIN   IOPORT_CREATE_PIN(PORTF, 1)
#define GREEN_LED_PIN IOPORT_CREATE_PIN(PORTD, 0)
#define BLUE_LED_PIN  IOPORT_CREATE_PIN(PORTD, 1)

#define RED_PWM_PORT      PWM_TCF0
#define RED_PWM_CHANNEL   PWM_CH_B
#define GREEN_PWM_PORT    PWM_TCD0
#define GREEN_PWM_CHANNEL PWM_CH_A
#define BLUE_PWM_PORT     PWM_TCD0
#define BLUE_PWM_CHANNEL  PWM_CH_B

// Bluetooth definitions:
#define BT_RESET_PIN         IOPORT_CREATE_PIN(PORTE, 0)
#define BT_RTS_PIN           IOPORT_CREATE_PIN(PORTE, 1)
#define BT_RX_PIN            IOPORT_CREATE_PIN(PORTE, 2)
#define BT_TX_PIN            IOPORT_CREATE_PIN(PORTE, 3)
#define BT_CTS_PIN           IOPORT_CREATE_PIN(PORTE, 4)
#define BT_ENABLE_PIN		 IOPORT_CREATE_PIN(PORTE, 5)
#define BT_STATUS_PIN        IOPORT_CREATE_PIN(PORTE, 6)
#define BT_FACTORY_RESET_PIN IOPORT_CREATE_PIN(PORTF, 0)

// Power definitions:
#define CHRG_STAT_PIN      IOPORT_CREATE_PIN(PORTC, 0)
#define POWER_SWITCH_PIN   IOPORT_CREATE_PIN(PORTC, 1)
#define BATTERY_DETECT_PIN IOPORT_CREATE_PIN(PORTA, 3)

#define BATTERY_DETECT_ADC ADCCH_POS_PIN3

// USB definitions:
#define USB_5V_PIN    IOPORT_CREATE_PIN(PORTF, 4)
#define USB_RESET_PIN IOPORT_CREATE_PIN(PORTE, 7)
#define USB_TX_PIN    IOPORT_CREATE_PIN(PORTC, 3)
#define USB_RX_PIN    IOPORT_CREATE_PIN(PORTC, 2)

// Debug definitions:
#define DEBUG_TX_PIN IOPORT_CREATE_PIN(PORTD, 3)
#define DEBUG_RX_PIN IOPORT_CREATE_PIN(PORTD, 2)

// SysTick definitions:
#define SYSTICK_TIMER     &TCC1
#define SYSTICK_FREQUENCY 1000

#define IsManufactureTestMode IOPORT_CREATE_PIN(PORTD, 7)
#define DRONE_GPIO_0 IOPORT_CREATE_PIN(PORTC, 4)
#define DRONE_GPIO_1 IOPORT_CREATE_PIN(PORTC, 5)
#define DRONE_GPIO_2 IOPORT_CREATE_PIN(PORTC, 6)
#define DRONE_GPIO_3 IOPORT_CREATE_PIN(PORTC, 7)

#endif // USER_BOARD_H
