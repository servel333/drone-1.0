/*
 * keyboard_ios.h
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

#ifndef KEYBOARD_IOS_H_
#define KEYBOARD_IOS_H_

#include "buttons.h"

void keyboardiOSInit(struct ControllerConfiguration *cc);
void keyboardiOSButtonHandler(enum ButtonName name, enum ButtonState state);

#endif /* KEYBOARD_IOS_H_ */