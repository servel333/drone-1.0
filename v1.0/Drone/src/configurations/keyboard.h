/*
 * keyboard.h
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

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "buttons.h"
#include "joysticks.h"

void keyboardInit(struct ControllerConfiguration *cc);
void keyboardButtonHandler(enum ButtonName name, enum ButtonState state);
void keyboardJoystickHandler(enum JoystickName name, uint16_t x, uint16_t y);

#endif /* KEYBOARD_H_ */