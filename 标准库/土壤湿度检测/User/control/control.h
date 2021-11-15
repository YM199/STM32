#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "bsp_SysTick.h"
//�̵�����������
#define CONTROL_GPIO_CLK   RCC_APB2Periph_GPIOA
#define CONTROL_Pin        GPIO_Pin_4
#define CONTROL_GPIO       GPIOA

//����������
#define BUZZER_CLK         RCC_APB2Periph_GPIOA
#define BUZZER_Pin         GPIO_Pin_5
#define BUZZER_GPIO        GPIOA
void control_Init(void);
void BUZZER_DI(void);
void BUZZER_DIDI(void);
#endif

