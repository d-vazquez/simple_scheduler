

#include <stdint.h>


//*****************************************************************************
//
// Definitions
//
//*****************************************************************************


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
uint64_t iddle_ticks = 0;


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
void idle_Task()
{
    while(1)
    {
        iddle_ticks++;
    }
}


