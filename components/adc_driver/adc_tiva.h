
#ifndef _ADC_TIVA_H_
#define _ADC_TIVA_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


void InitADC0(void);
void ADCReadPercent(uint16_t *percent_value);


#endif  // _ADC_TIVA_H_
