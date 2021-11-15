#ifndef __HC05_H
#define __HC05_H

#include "sys.h"

//����ģ��GPIO��ز�����һ���װ
//**********************************************************************************
#define RCC_STATE  RCC_APB2Periph_GPIOA
#define RCC_EN		RCC_APB2Periph_GPIOC
#define STATE_Pin GPIO_Pin_4 
#define EN_Pin GPIO_Pin_4
//**********************************************************************************

#define HC05_EN  	    PCout(4) 	//��������EN�ź�
#define HC05_STATE  	PAin(4)		//��������״̬�ź�


u8 HC05_Init(void);
//void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	
#endif