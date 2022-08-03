
#ifndef _THREADING_H_
#define _THREADING_H_


#include <stdint.h>


#define TASK_STK    uint32_t

typedef enum TASK_STATUS
{
    TASK_SUSPENDED = 0xAA,
    TASK_ACTIVE = 0xCC,
}TASK_STATUS;

// Task properties
typedef struct
{
    uint32_t *TaskStack; // Stack is always first element
    void    (*Task )();
    char     *identifier;
    uint32_t TaskStatus;
} TCB_Type;

void     Scheduler_Init(void);
TCB_Type *Scheduler_Task_Create(char* identifier,
                           void (*fTask )(),
                           uint32_t *TaskStack,
                           uint32_t StackSize,
                           TASK_STATUS task_status);
void     Scheduler_Start(void);
void     Scheduler_Task_Suspend(TCB_Type *);
void     Scheduler_Task_Active(TCB_Type *);
uint64_t get_Scheduler_TickCount();
void     Scheduler_Task_Wait(uint32_t cycles);

#endif  // _THREADING_H_
