#ifndef _IIC_H
#define _IIC_H

#include "stm32f10x.h"

//-----------------IIC端口定义----------------  					   

#define RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define IIC_GPIO              GPIOA
#define IIC_SCL_Pin           GPIO_Pin_1
#define IIC_SDA_Pin           GPIO_Pin_2


#define IIC_SCL_0() GPIO_ResetBits(IIC_GPIO,IIC_SCL_Pin)//SDA IIC接口的时钟信号
#define IIC_SCL_1() GPIO_SetBits(IIC_GPIO,IIC_SCL_Pin)

#define IIC_SDA_0() GPIO_ResetBits(IIC_GPIO,IIC_SDA_Pin)//SCL IIC接口的数据信号
#define IIC_SDA_1() GPIO_SetBits(IIC_GPIO,IIC_SDA_Pin)

#define IIC_Read_SDA() GPIO_ReadInputDataBit(IIC_GPIO,IIC_SDA_Pin)
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(u8 IIC_Byte);
#endif

