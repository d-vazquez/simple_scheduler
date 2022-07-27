

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"
#include "idle_task.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define MAX_THREADS 10
#define ENTER_CRITICAL_SECTION  CPU_disableInterrupts();
#define EXIT_CRITICAL_SECTION   CPU_enableInterrupts();


//*****************************************************************************
//
// Internal functions
//
//*****************************************************************************
static uint32_t get_Scheduler_currentTask();
static void     set_Scheduler_currentTask(uint32_t param);
static uint32_t get_Scheduler_nextTask();
static void     Scheduler_Tick_Init();
static void     Scheduler_Tick_EnableInt();


//*****************************************************************************
//
// Internal variable types
//
//*****************************************************************************
// Task properties
typedef struct
{
    uint32_t *TaskStack; // Stack is always first element
    void    (*Task )();
    char     *identifier;
    uint32_t Attributes0;
    uint32_t Attributes1;
    uint32_t Attributes2;
    uint32_t Attributes3;
    uint32_t Attributes4;
} TCB_Type;

// Scheduler properties
typedef struct
{
    uint32_t TaskdIndex;
    uint32_t TaskCount;
    uint64_t TickCount;
    TCB_Type TCB[MAX_THREADS];
    uint32_t TCB_Size;
} scheduler_type;


//*****************************************************************************
//
// Internal memory (variables)
//
//*****************************************************************************
static scheduler_type scheduler =
{
    .TaskdIndex= 0,
    .TaskCount = 0,
    .TickCount = 0,
    .TCB_Size = sizeof(TCB_Type),
};

// context switch is a naked c/asm function, locals cannot be declared,
// and struct members cant be accessed, need some memory to cache information
static uint32_t current_task;
static uint32_t next_task;
static uint32_t scheduler_TCB_Size;
static TCB_Type *scheduler_TCB;



//*****************************************************************************
//
// Functions
//
//*****************************************************************************
void Scheduler_Init()
{
    // Setup CPU systick
    Scheduler_Tick_Init();

    // Create idle task
    Scheduler_Task_Create(  IDLE_TASK_NAME,
                            idle_Task,
                            idle_task_stack,
                            IDLE_TASK_STACK_SIZE);
}


void Scheduler_Task_Create(char* identifier, void (*fTask )(), uint32_t *TaskStack, uint32_t StackSize)
{
    ENTER_CRITICAL_SECTION

    // Check we are not our of Thread count
    if (scheduler.TaskCount < MAX_THREADS)
    {
        // Pass task name
        scheduler.TCB[scheduler.TaskCount].identifier = identifier;

        // Assign task function to TCB
        scheduler.TCB[scheduler.TaskCount].Task = fTask;

        // Initialize thread stack with known values (will initialize with 'STCK')
        // Help us see what stack has been touched
        for (uint32_t indx = 0; indx < StackSize; indx++)
        {
            TaskStack[indx] = 0xA5A5A5A5;
        }

        // Start task stack, simulating as if it came from an exception (ISR).
        // From ARM M0 documentation, the ISR frame is (starting from TOP)
        TaskStack[StackSize-1] = (1 << 24);         // xPSR - arithmetic flags
                                                    // to zero and Thumb bit is set
        TaskStack[StackSize-2] = (uint32_t)fTask;   // PC - Start of task
        TaskStack[StackSize-3] = 0;                 // LR
        TaskStack[StackSize-4] = 0;                 // R12
        TaskStack[StackSize-5] = 0;                 // R3
        TaskStack[StackSize-6] = 0;                 // R2
        TaskStack[StackSize-7] = 0;                 // R1
        TaskStack[StackSize-8] = 0;                 // R0

        // After the call frame is set, we also want to store all CPU register,
        // the order is intended for how we work our context switch
        TaskStack[StackSize-9]  = 0;                // R11
        TaskStack[StackSize-10] = 0;                // R10
        TaskStack[StackSize-11] = 0;                // R9
        TaskStack[StackSize-12] = 0;                // R8
        TaskStack[StackSize-13] = 0;                // R7
        TaskStack[StackSize-14] = 0;                // R6
        TaskStack[StackSize-15] = 0;                // R5
        TaskStack[StackSize-16] = 0;                // R4

        // TCB Stak pointing to the top of our simulated frame
        scheduler.TCB[scheduler.TaskCount].TaskStack = &TaskStack[StackSize-16];

        // Task has been initiated, increase counter
        scheduler.TaskCount++;
    }

    EXIT_CRITICAL_SECTION
}


void Scheduler_Tick_Init()
{
    CPU_SysTick_Init();
}


void Scheduler_Tick_EnableInt()
{
    CPU_SysTick_EnableInt();
}


void Scheduler_Start()
{
    ENTER_CRITICAL_SECTION

    // Enable Interruptions for Tick
    Scheduler_Tick_EnableInt();

    // Cache TCB info for context switch
    scheduler_TCB_Size = scheduler.TCB_Size;
    scheduler_TCB = scheduler.TCB;

    // start the thread stack 'PSP' with the first thread
    asm(" LDR   R0,     =scheduler_TCB  "); // point to start of TCB array
    asm(" LDR   R0,     [R0]            "); // scheduler_TCB is a pointer to TCB, so deference
    asm(" LDR   R0,     [R0]            "); // Task stack is first element, access it
    asm(" ADD   R0,     #32             "); // Adjust first thread stack, we will iddle until
                                            // isr, in the isr we are expected to have a fram
                                            // already stacked, the stack will have
                                            // top -> bot { R0 ,R1, R2, R3, R12, LR, PC, xPCR }
                                            // thus we advance the pointer 8*4 = 32

    asm(" MSR   PSP,    R0              "); // Move thread0 stack to be ready for context swtch

    // Start context switch ISR
    EXIT_CRITICAL_SECTION

    // Overflowing ThreadIndex to start with Thread0
    //set_Scheduler_currentTask((uint32_t)-1);

    while(1)
    {
        // We will be here at the start, when we start switching we will never come back
    }
}


// Naked attribute, keep ISR nice and short, only touch R0 and R1
__attribute__((naked)) void Scheduler_Tick_process(void)
{
    // Enter critical section
    ENTER_CRITICAL_SECTION

    // Increment tick count
    scheduler.TickCount++;

    // get current running task
    current_task = get_Scheduler_currentTask();

    // Get next task to run
    next_task = get_Scheduler_nextTask();

    // Set current running task (for next iteration)
    set_Scheduler_currentTask(next_task);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // START CONTEXT SWITCH, NO FURTHER FUNCTION CALLS AT THIS PIOINT  !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // push current task context into stack
    asm(" MRS   R0,     PSP             "); // Get current process stack pointer
    asm(" SUB   R0,     #32             "); // Make room for the other registers : R4-R11 = 8 x 4 = 32 bytes

    asm(" STMIA R0!,    { R4-R7 }       "); // Multiple Store into the current stack pointer

    asm(" MOV   R4,     R8              "); // Get rest of registers
    asm(" MOV   R5,     R9              ");
    asm(" MOV   R6,     R10             ");
    asm(" MOV   R7,     R11             ");
    asm(" STMIA R0!,    { R4-R7 }       "); // Multiple Store into the current stack pointer

    // Get TCB for current running task
    asm(" LDR   R0, =scheduler_TCB_Size "); // Get Size
    asm(" LDR   R0,     [R0]            ");
    asm(" LDR   R1,     =current_task   "); // Get current index
    asm(" LDR   R1,     [R1]            ");

    asm(" MUL   R1,     R0,R1           "); // size * index to get the offset of the TCB array
    asm(" LDR   R0,     =scheduler_TCB  "); // Get the start of TCB array
    asm(" LDR   R0,     [R0]            "); // scheduler_TCB is a pointer to TCB, so deference
    asm(" ADD   R1,     R0,R1           "); // point to current thread TCB
    asm(" MRS   R0,     PSP             "); // get Thread A stack pointer

    // Save current thread stack pointer (adjusted for new registers being pushed)
    asm(" SUB   R0,     #32             "); // Adjust for the other registers : R4-R11 = 8 x 4 = 32 bytes
    asm(" STR   R0,     [R1]            "); // Save current thread Stack pointer to the TCB
                                            // (first entry = Saved stack pointer)

    // Locate the Thread Control Block (TCB) for Next thread (do the same in opposite order)
    asm(" LDR   R0, =scheduler_TCB_Size "); // Get size
    asm(" LDR   R0,     [R0]            ");
    asm(" LDR   R1,     =next_task      "); // Get new index
    asm(" LDR   R1,     [R1]            ");
    asm(" MUL   R1,     R0,R1           "); // Calculate offset of next task

    asm(" LDR   R0,     =scheduler_TCB  "); // Go to the start of TCB array
    asm(" LDR   R0,     [R0]            "); // scheduler_TCB is a pointer to TCB, so deference
    asm(" ADD   R1,     R0,R1           "); // Add offset, now R1 points to the next task TCB

    // Pop the next thread stack and registers
    asm(" LDR   R0,     [R1]            "); // Top of TCB is the thread stack, get it
    asm(" ADD   R0,     #16             "); // Skip R4 - R7
    asm(" LDMIA R0!,    {R4-R7}         "); // Read R8 - R11
    asm(" MOV   R8,     R4              "); // Move R8 - R11 to its actual destination
    asm(" MOV   R9,     R5              ");
    asm(" MOV   R10,    R6              ");
    asm(" MOV   R11,    R7              ");
    asm(" LDR   R0,     [R1]            "); // Read again Thread stack
    asm(" LDMIA R0!,    {R4-R7}         "); // Read R4 - R7
    asm(" ADD   R0,     #16             "); // R0 advanced 4 register, advance it 4 more to skip R8 - R11
    asm(" MSR   PSP,    R0              "); // Write new stack pointer to SP

    asm(" MOV   R0,     #0xFFFFFFFD     ");
    asm(" MOV   LR,     R0              "); // Force LR as 0xFFFFFFFD,
                                            // this will tell CPU to return and load PSP into the stack,
                                            // the return function of the new task will be popped

    EXIT_CRITICAL_SECTION                   // exit critical section

    asm(" BX    LR                      "); // return to Next thread
}


uint64_t get_Scheduler_TickCount(void)
{
    return scheduler.TickCount;
}


static void set_Scheduler_currentTask(uint32_t param)
{
    scheduler.TaskdIndex = param;
}


static uint32_t get_Scheduler_currentTask()
{
    return scheduler.TaskdIndex;
}


static uint32_t get_Scheduler_nextTask()
{
    uint32_t next_task = get_Scheduler_currentTask() + 1;

    // Cap to max thread
    return (next_task >= scheduler.TaskCount) ? 0 : next_task ;
}

