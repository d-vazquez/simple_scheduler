

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"


//*****************************************************************************
//
// Definitions
//
//*****************************************************************************


//*****************************************************************************
//
// Internal functions
//
//*****************************************************************************
static void blue_led_InitHW(void);


//*****************************************************************************
//
// Internal variable types
//
//*****************************************************************************


//*****************************************************************************
//
// Internal memory (variables)
//
//*****************************************************************************


//*****************************************************************************
//
// Functions
//
//*****************************************************************************
void blue_led_Task()
{
    uint32_t task_period = 500;
    uint64_t current_tick = get_Scheduler_TickCount();
    uint64_t timeout_tick = get_Scheduler_TickCount() + task_period;

    blue_led_InitHW();

    while(1)
    {
        current_tick = get_Scheduler_TickCount();
        if (current_tick >= timeout_tick)
        {
            // toggle the blue led
            timeout_tick = current_tick + task_period;

            // critical section
            CPU_disableInterrupts();

            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)) ? 0x00 : GPIO_PIN_2;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 , toogle);

            CPU_enableInterrupts();
        }
    }
}

static void blue_led_InitHW(void)
{
    // Enable PORT for segments gate clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait clock to be stable
    while ((HWREG(SYSCTL_PRGPIO) & SYSCTL_PRGPIO_R5) == 0)
    {
    }

    // Enable PORT pins as Output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Write all PINS low
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);

}



//// The stack size is expressed in 32 bit words
//#define STACK_SIZE 64
//TASK_STK Thread1Stack[STACK_SIZE];
//TASK_STK Thread2Stack[STACK_SIZE];
//TASK_STK Thread3Stack[STACK_SIZE];
//
//void ConfigPins(void);
//
//void Thread1(void)
//{
//    uint32_t task_period = 500;
//    uint64_t current_tick = get_Scheduler_TickCount();
//    uint64_t timeout_tick = get_Scheduler_TickCount() + task_period;
//    while(1)
//    {
//        current_tick = get_Scheduler_TickCount();
//        if (current_tick >= timeout_tick)
//        {
//            // toggle the blue led
//            timeout_tick = current_tick + task_period;
//
//            // critical section
//            CPU_disableInterrupts();
//
//            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)) ? 0x00 : GPIO_PIN_2;
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 , toogle);
//
//            CPU_enableInterrupts();
//        }
//    }
//}
//void Thread2(void)
//{
//    static uint32_t Counter2 = 0;
//    while(1)
//    {
//        Counter2++;
//        if (Counter2 >200000)
//        {// toggle the red led
//            Counter2 = 0;
//            CPU_disableInterrupts();
//
//            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)) ? 0x00 : GPIO_PIN_1;
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , toogle);
//
//            CPU_enableInterrupts();
//        }
//    }
//}
//void Thread3(void)
//{
//    static uint32_t Counter3 = 0;
//
//    while(1)
//    {
//        Counter3++;
//        if (Counter3 > 400000)
//        {   // toggle the green led
//            Counter3 = 0;
//            CPU_disableInterrupts();
//
//            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)) ? 0x00 : GPIO_PIN_3;
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 , toogle);
//
//            CPU_enableInterrupts();
//
//        }
//
//    }
//}

