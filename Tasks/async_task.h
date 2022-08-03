
#ifndef _ASYNC_TASK_H_
#define _ASYNC_TASK_H_

#include "threading.h"

#define ASYNC_TASK_STACK_SIZE    64
#define ASYNC_TASK_NAME          "ASYNC Task"

TCB_Type *async_task_tcb;

TASK_STK async_task_stack[ASYNC_TASK_STACK_SIZE];

void async_Task();

#endif  // _ASYNC_TASK_H_
