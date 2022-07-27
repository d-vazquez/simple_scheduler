
#ifndef _REDLED_TASK_H_
#define _REDLED_TASK_H_

#include "threading.h"

#define RED_LED_TASK_STACK_SIZE    64
#define RED_LED_TASK_NAME          "Red led Task"

TASK_STK red_led_task_stack[RED_LED_TASK_STACK_SIZE];

void red_led_Task();

#endif  // _REDLED_TASK_H_
