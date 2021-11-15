#ifndef ultrasonic_H
#define	ultrasonic_H

#include "stm32f10x.h"
extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
void TIM5_Cap_Init(u16 arr,u16 psc);
void UltrasonicWave_StartMeasure(void);
float rang(void);
void UltrasonicWave_Init(void);
#endif


