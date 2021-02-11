#ifndef _ENCODER_H
#define _ENCODER_H

#include "stm32f10x.h"

#define Encoder_CH1   GPIO_Pin_6
#define Encoder_CH2   GPIO_Pin_7
#define Encoder_GPIO  GPIOA

#define Encoder_Tim   TIM3
#define Parameter     390

void Encoder_Init(void);

#endif
