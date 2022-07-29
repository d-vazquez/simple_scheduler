
#ifndef _LCD_TASK_H_
#define _LCD_TASK_H_

#include "threading.h"

#define LCD_TASK_STACK_SIZE    64*2
#define LCD_TASK_NAME          "LCD Task"

TASK_STK lcd_task_stack[LCD_TASK_STACK_SIZE];

void lcd_Task();

#endif  // _LCD_TASK_H_
