
#ifndef _BLUELED_TASK_H_
#define _BLUELED_TASK_H_

#include "threading.h"

#define BLUE_LED_TASK_STACK_SIZE    64
#define BLUE_LED_TASK_NAME          "Blue led Task"

TASK_STK blue_led_task_stack[BLUE_LED_TASK_STACK_SIZE];

void blue_led_Task();

#endif  // _BLUELED_TASK_H_
