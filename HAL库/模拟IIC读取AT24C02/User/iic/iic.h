#ifndef __LED_H
#define	__LED_H

#include "stm32f1xx.h"

#define dev_addr  0xA0//7λ��ַ
#define write_bit       0
#define read_bit        1

//���Ŷ���
/*******************************************************/
#define IIC_SCL_PIN             GPIO_PIN_6
#define IIC_SDA_PIN           	GPIO_PIN_7
#define IIC_GPIO_PORT           GPIOB                     
#define IIC_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* ���κ꣬��������������һ��ʹ�� */
#define IIC_SCL(a)			HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SCL_PIN,a)
#define IIC_SDA(a)			HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SDA_PIN,a)
#define IIC_SDA_SCL(a)	HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SCL_PIN|IIC_SDA_PIN,a)
#define IIC_Read_SDA() 	HAL_GPIO_ReadPin(IIC_GPIO_PORT,IIC_SDA_PIN)






				

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Write_Byte(uint8_t IIC_Byte);
uint8_t IIC_Byte_Read(void);
uint8_t Ran_Read(uint8_t addr);
void IIC_Wait_Ack(void);
void IIC_No_Ack(void);
void IIC_Ack(void);
void IIC_GPIO_Config(void);
void check_device(uint8_t addr);
#endif /* __LED_H */
