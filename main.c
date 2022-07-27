///////////////////////////////////////////////////////////////////////////////
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
///////////////////////////////////////////////////////////////////////////////


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "cpu_tiva.h"
#include "threading.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"



// The stack size is expressed in 32 bit words
#define STACK_SIZE 64
uint32_t Thread1Stack[STACK_SIZE];
uint32_t Thread2Stack[STACK_SIZE];
uint32_t Thread3Stack[STACK_SIZE];

void ConfigPins(void);

void Thread1(void)
{
    uint64_t current_tick = get_Scheduler_TickCount();
    uint64_t timeout_tick = get_Scheduler_TickCount() + 1000;
    while(1)
    {
        current_tick = get_Scheduler_TickCount();
        if (current_tick >= timeout_tick)
        {
            // toggle the blue led
            timeout_tick = current_tick + 1000;

            // critical section
            CPU_disableInterrupts();

            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)) ? 0x00 : GPIO_PIN_2;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 , toogle);

            CPU_enableInterrupts();
        }
    }
}
void Thread2(void)
{
    static uint32_t Counter2 = 0;
    while(1)
    {
        Counter2++;
        if (Counter2 >200000)
        {// toggle the red led
            Counter2 = 0;
            CPU_disableInterrupts();

            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)) ? 0x00 : GPIO_PIN_1;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , toogle);

            CPU_enableInterrupts();
        }
    }
}
void Thread3(void)
{
    static uint32_t Counter3 = 0;

    while(1)
    {
        Counter3++;
        if (Counter3 > 400000)
        {   // toggle the green led
            Counter3 = 0;
            CPU_disableInterrupts();

            uint32_t toogle = (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)) ? 0x00 : GPIO_PIN_3;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 , toogle);

            CPU_enableInterrupts();

        }

    }
}


int main(void)
{
    CPU_Clk_Init();

    printfInit();
    printf("Context switch v2.9 \r\n");

    ConfigPins();

    Scheduler_Init();

    Scheduler_Task_Create(Thread1,Thread1Stack,STACK_SIZE);
//    createThread(Thread2,Thread2Stack,STACK_SIZE);
//    createThread(Thread3,Thread3Stack,STACK_SIZE);

    Scheduler_Start(); // start the thread switching (this does not return)

    while(1)
    {

    }
    return 0;
}


void ConfigPins(void)
{
    // Enable PORT for segments gate clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait clock to be stable
    while ((HWREG(SYSCTL_PRGPIO) & SYSCTL_PRGPIO_R5) == 0)
    {
    }

    // Enable PORT pins as Output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Write all PINS low
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);

}

