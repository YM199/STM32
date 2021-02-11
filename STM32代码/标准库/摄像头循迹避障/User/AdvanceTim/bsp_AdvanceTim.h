#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"

/************TIM5��������************/
#define    GENERAL_TIM                   TIM5
#define    TIM5_CH1_Pin                  GPIO_Pin_0
#define    TIM5_CH2_Pin                  GPIO_Pin_1
#define    TIM5_GPIO                     GPIOA
#define    TIM5_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define    GENERAL_TIM_IRQ               TIM5_IRQn
#define    GENERAL_TIM_IRQHandler        TIM5_IRQHandler


//PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR(Period)+1)*(PSC+1) / 72M     1M = 1000000
#define    GENERAL_TIM5_Period     100-1
#define    GENERAL_TIM5_PSC        7.2-1   //10KHZ
#define    GERENAL_TIM5_PULSE      30
#define    GENERAL_TIM5            TIM5
/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (2000-1)
#define            ADVANCE_TIM_PSC               (720-1)
#define            ADVANCE_TIM_PULSE             140

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 ����Ƚ�ͨ��1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8



// TIM1 ����Ƚ�ͨ��2
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH2_PORT          GPIOA
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_11



// TIM1 ����Ƚ�ͨ����ɲ��ͨ��
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12

/**************************��������********************************/

void ADVANCE_TIM_Init(void);
void T5_Init(void);

#endif	/* __BSP_ADVANCETIME_H */


