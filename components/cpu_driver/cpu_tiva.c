

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "driverlib/cpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"


#define CPU_CLOCK                       80000000u
#define CPU_CYCLE_TO_MS(x)              (x*(CPU_CLOCK/1000u))


void CPU_Clk_Init()
{
    // Set the clocking to run at max 80 MHz
    // external OSC (16 MHz) -> PLL (200 MHz) -> DIV2.5 --> 80 MHz total
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Wait clock stabilize
    SysCtlDelay(4096);
}

void CPU_Waitms(uint32_t millis)
{
    // Wait for the required cycles per the millis parameter
    SysCtlDelay(CPU_CYCLE_TO_MS(millis));
}

// Enable systick, configured for tick every 1ms
void CPU_SysTick_Init()
{
    // Enable systick, clock source is system clock (CPU_CLOCK)
    SysTickEnable();

    // reload with the clock freq div 1000 to get a
    // interrupt every millisecond
    SysTickPeriodSet(CPU_CYCLE_TO_MS(1));
}

void CPU_SysTick_EnableInt()
{
    SysTickIntEnable();
}

__attribute__((naked)) void CPU_SysTick_Handler()
{
    Scheduler_Tick_process();
}


