
#ifndef _CPU_TIVA_H_
#define _CPU_TIVA_H_


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#define CPU_enableInterrupts()  asm(" cpsie i ")
#define CPU_disableInterrupts() asm(" cpsid i ")


void CPU_Clk_Init();
void CPU_Waitms(uint32_t millis);
void CPU_SysTick_Init();
void CPU_SysTick_EnableInt();
void CPU_SysTick_Handler(void);


#endif  // _CPU_TIVA_H_
