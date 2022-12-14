

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "adc_tiva.h"

extern TCB_Type *async_task_tcb;

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define TASK_PERIOD     10
#define SWITCH_GPIO    GPIO_PIN_6

//*****************************************************************************
//
// Internal functions
//
//*****************************************************************************
static void switch_InitHW(void);


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
static uint16_t potentiometer_percent = 0;
static uint16_t potentiometer_raw = 0;
static uint16_t switch_value_raw = 0;


//*****************************************************************************
//
// Functions
//
//*****************************************************************************
void an_in_Task()
{
    // Init ADC module
    InitADC0();
    switch_InitHW();

    uint64_t current_tick = get_Scheduler_TickCount();
    uint64_t timeout_tick = get_Scheduler_TickCount() + TASK_PERIOD;

    while(1)
    {
        current_tick = get_Scheduler_TickCount();
        if (current_tick >= timeout_tick)
        {
            // toggle the blue led
            timeout_tick = current_tick + TASK_PERIOD;

            // critical section
            CPU_disableInterrupts();

            // get adc value
            ADCReadPercent(&potentiometer_raw);
            // read switch
            switch_value_raw = GPIOPinRead(GPIO_PORTA_BASE, SWITCH_GPIO);

            CPU_enableInterrupts();

            // Convert percent (adc is 12bit, so read value divided by 2^12 - 1, by 100 for percent)
            potentiometer_percent = (uint32_t)(potentiometer_raw / 40.950f);

            // switch pressed
            if(!switch_value_raw)
            {
                // DEBOUNCE
                Scheduler_Task_Wait(100);

                CPU_disableInterrupts();
                switch_value_raw = GPIOPinRead(GPIO_PORTA_BASE, SWITCH_GPIO);
                CPU_enableInterrupts();

                if(!switch_value_raw)
                {
                    Scheduler_Task_Active(async_task_tcb);
                }
            }
        }
    }
}

uint16_t getPotPercent()
{
    return potentiometer_percent;
}

static void switch_InitHW(void)
{
    // Enable PORT for segments gate clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Wait clock to be stable
    while ((HWREG(SYSCTL_PRGPIO) & SYSCTL_PRGPIO_R5) == 0)
    {
    }

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, SWITCH_GPIO);
}
