#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"


#define Hang_00_L  GPIO_ResetBits(GPIOG, GPIO_Pin_6)//行00
#define Hang_00_H  GPIO_SetBits(GPIOG, GPIO_Pin_6)
 
#define Hang_01_L  GPIO_ResetBits(GPIOG, GPIO_Pin_4)//行01
#define Hang_01_H  GPIO_SetBits(GPIOG, GPIO_Pin_4)
 
#define Hang_02_L  GPIO_ResetBits(GPIOG, GPIO_Pin_2)//行02
#define Hang_02_H  GPIO_SetBits(GPIOG, GPIO_Pin_2)
 
#define Hang_03_L  GPIO_ResetBits(GPIOD, GPIO_Pin_14)//行03
#define Hang_03_H  GPIO_SetBits(GPIOD, GPIO_Pin_14)
 
#define Lie_00_V GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)//列00
#define Lie_01_V GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//列01
#define Lie_02_V GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//列02
#define Lie_03_V GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//列03

#define  jpys 1   //宏定义，约定jpys==20，方便以后程序移植


void Key_Config(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数

#endif


