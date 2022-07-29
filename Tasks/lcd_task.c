

#include <stdint.h>
#include "threading.h"
#include "cpu_tiva.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "SSD1306_driver.h"
#include "an_in_task.h"

//*****************************************************************************
//
// Definitions
//
//*****************************************************************************
#define TASK_PERIOD     30

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


//*****************************************************************************
//
// Functions
//
//*****************************************************************************
void lcd_Task()
{
    // Init Screen
    InitScreen();
    PrintScreen();

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

            // print to screen
            FillbarScreen(getPotPercent());
            PrintScreen();


            CPU_enableInterrupts();
        }
    }
}



