

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
#define BLUE_LED_GPIO   GPIO_PIN_2
#define TASK_PERIOD     500

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
    uint64_t current_tick = get_Scheduler_TickCount();
    uint64_t timeout_tick = get_Scheduler_TickCount() + TASK_PERIOD;

    blue_led_InitHW();

    while(1)
    {
        current_tick = get_Scheduler_TickCount();
        if (current_tick >= timeout_tick)
        {
            // toggle the blue led
            timeout_tick = current_tick + TASK_PERIOD;

            // critical section
            CPU_disableInterrupts();

            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, BLUE_LED_GPIO)) ? 0x00 : BLUE_LED_GPIO;
            GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED_GPIO , toogle);

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
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, BLUE_LED_GPIO);

    // Write all PINS low
    GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED_GPIO, 0x00);

}

