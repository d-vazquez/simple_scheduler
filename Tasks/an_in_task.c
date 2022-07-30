

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "adc_tiva.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define TASK_PERIOD     10

//*****************************************************************************
//
// Internal functions
//
//*****************************************************************************


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


//*****************************************************************************
//
// Functions
//
//*****************************************************************************
void an_in_Task()
{
    // Init ADC module
    InitADC0();

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

            uint16_t potentiometer_raw = 0;

            // get value
            ADCReadPercent(&potentiometer_raw);

            // Convert percent (adc is 12bit, so read value divided by 2^12 - 1, by 100 for percent)
            potentiometer_percent = (uint32_t)(potentiometer_raw / 40.950f);

            CPU_enableInterrupts();
        }
    }
}

uint16_t getPotPercent()
{
    return potentiometer_percent;
}


