#ifndef _STM32F10x_IT_H_
#define _STM32F10x_IT_H_

#ifdef __cplusplus
 extern "C" {
#endif 
 
#include "stm32f10x.h"
 
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SVC_Handler(void);

#ifdef __cplusplus
}
#endif

#endif 


