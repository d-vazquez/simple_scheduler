
#ifndef _THREADING_H_
#define _THREADING_H_


#include <stdint.h>


void Scheduler_Init(void);
void Scheduler_Task_Create(void (*ThreadFn )(), uint32_t *ThreadStack, uint32_t StackSize);
void Scheduler_Start(void);
uint64_t get_Scheduler_TickCount();


#endif  // _THREADING_H_
