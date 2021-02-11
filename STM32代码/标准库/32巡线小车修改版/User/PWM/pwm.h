#ifndef     __PWM_H_
#define     __PWM_H_

#include "stm32f10x.h"

#define    TIM5_GPIO_CLK   RCC_APB2Periph_GPIOA

#define    TIM5_CH1_Pin   GPIO_Pin_0
#define    TIM5_CH2_Pin   GPIO_Pin_1
#define    TIM5_CH3_Pin   GPIO_Pin_2
#define    TIM5_CH4_Pin   GPIO_Pin_3

#define    TIM5_GPIO      GPIOA

//PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR(Period)+1)*(PSC+1) / 72M     1M = 1000000
#define    GENERAL_TIM5_Period     2000-1
#define    GENERAL_TIM5_PSC        720-1
#define    GERENAL_TIM5_PULSE      50
#define    GENERAL_TIM5            TIM5


void T5_Init(void);

#endif      //__PWM_H_


