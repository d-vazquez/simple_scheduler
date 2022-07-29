
#ifndef _AN_IN_TASK_H_
#define _AN_IN_TASK_H_

#include "threading.h"

#define AN_IN_TASK_STACK_SIZE    64*2
#define AN_IN_TASK_NAME          "AN_IN Task"

TASK_STK an_in_task_stack[AN_IN_TASK_STACK_SIZE];

void an_in_Task();
uint16_t getPotPercent();

#endif  // _AN_IN_TASK_H_
