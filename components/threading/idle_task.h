
#ifndef _IDLE_TASK_H_
#define _IDLE_TASK_H_

#include "threading.h"

#define IDLE_TASK_STACK_SIZE    64
#define IDLE_TASK_NAME          "Idle led Task"

TASK_STK idle_task_stack[IDLE_TASK_STACK_SIZE];

void idle_Task();

#endif  // _IDLE_TASK_H_
