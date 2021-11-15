#ifndef __ADC_H
#define	__ADC_H
#include "stm32f10x.h"
void ADCx_Init(void);
void average(u16 ADC_ConvertedValue);
#endif /* __ADC_H */

