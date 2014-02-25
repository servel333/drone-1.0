/*
 * watchdog.h
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

/** @file
 * Definitions for watchdog functions.
 *
 * @author Alan Widmer
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H
#include <stdbool.h>
#include <stdint.h>
#include <asf.h>

/* Definitions for the maximum expected time for a subsystem to execute (in seconds) */
#define MODEM_RESPONSE_WATCHDOG_RESET 100
#define MAIN_LOOP_WATCHDOG_RESET 100
#define ACCELEROMETER_WATCHDOG_RESET 2
#define WATCHDOG_INITIALIZE_RESET 15

typedef enum  taskName {
  TASK_MAIN,
  TASK_ACCEL,
  TASK_MODEM
} taskDef;

void WatchdogInit(void);
void WatchDogEnable(bool enable);
void WatchdogPet(void);
void WatchdogValidate(void);
void WatchdogMaxTimeout(void);
void WatchdogTaskRunning(taskDef task);

#endif /* WATCHDOG_H */
