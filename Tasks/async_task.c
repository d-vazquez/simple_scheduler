

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "async_task.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define TASK_PERIOD     1
#define ASYNC_LED_GPIO    GPIO_PIN_7

//*****************************************************************************
//
// Internal functions
//
//*****************************************************************************
static void async_InitHW(void);


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
void async_Task()
{
    async_InitHW();
    uint64_t current_tick = get_Scheduler_TickCount();
    uint64_t timeout_tick = get_Scheduler_TickCount() + TASK_PERIOD;

    while(1)
    {
        current_tick = get_Scheduler_TickCount();
        if (current_tick >= timeout_tick)
        {
            // toggle the blue led
            timeout_tick = current_tick + TASK_PERIOD;

            // toggle led
            uint32_t toogle = (GPIOPinRead(GPIO_PORTA_BASE, ASYNC_LED_GPIO)) ? 0x00 : ASYNC_LED_GPIO;
            GPIOPinWrite(GPIO_PORTA_BASE, ASYNC_LED_GPIO , toogle);

            Scheduler_Task_Suspend(async_task_tcb);
        }
    }
}

void async_InitHW()
{
    // Enable PORT for segments gate clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Wait clock to be stable
    while ((HWREG(SYSCTL_PRGPIO) & SYSCTL_PRGPIO_R5) == 0)
    {
    }

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, ASYNC_LED_GPIO);
    GPIOPinWrite(GPIO_PORTA_BASE, ASYNC_LED_GPIO, 0x00);
}

