

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


uint32_t adc0_mem_store[4];


void InitADC0(void)
{
    // The ADC0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable output pin
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7);

    // Configure to run on software trigger
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

    // Configure steps, all steps will read same channel
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0 | ADC_CTL_IE );
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH0 | ADC_CTL_IE );
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH0 | ADC_CTL_IE );
    ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    // Enable channel
    ADCSequenceEnable(ADC0_BASE, 2);

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 2);
}


void ADCReadPercent(uint16_t *measurement)
{
    // Trigger the ADC conversion.
    ADCProcessorTrigger(ADC0_BASE, 2);

    // Wait for conversion to be completed.
    while(!ADCIntStatus(ADC0_BASE, 2, false))
    {
    }

    // Clear the ADC interrupt flag.
    ADCIntClear(ADC0_BASE, 2);

    // Read ADC Value.
    ADCSequenceDataGet(ADC0_BASE, 2, adc0_mem_store);

    // Get average
    *measurement = (adc0_mem_store[0] + adc0_mem_store[1] + adc0_mem_store[2] + adc0_mem_store[3])/4;
}

