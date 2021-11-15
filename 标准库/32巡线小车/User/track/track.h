#ifndef TRACK_H
#define TRACK_H
 
#include "stm32f10x.h"
 
 
//Track IO�ڵ�ʱ��
#define TRACK_GPIO_CLOCK RCC_APB2Periph_GPIOB
 
//2·����ģ��IO��
#define Track1           GPIO_Pin_12
#define Track2           GPIO_Pin_13
  
#define Track_GPIO       GPIOB
 
 
#define GO         200
#define STOP       0
void Track_GPIO_Config(void);
uint8_t Track_Scan(void);
 
#endif // TRACK_H
 
 


