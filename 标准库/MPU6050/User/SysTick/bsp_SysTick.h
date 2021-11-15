#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);

int get_tick_count(unsigned long *count);
void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);


#endif /* __SYSTICK_H */
