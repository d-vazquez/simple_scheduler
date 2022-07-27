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
#include "blue_led_task.h"


int main(void)
{
    CPU_Clk_Init();

    printfInit();
    printf("Context switch v3.1 \r\n");

    Scheduler_Init();

    Scheduler_Task_Create(  BLUE_LED_TASK_NAME,
                            blue_led_Task,
                            blue_led_task_stack,
                            BLUE_LED_TASK_STACK_SIZE);

    Scheduler_Start(); // start the thread switching (this does not return)

    // Should not reach here
    return -1;
}



