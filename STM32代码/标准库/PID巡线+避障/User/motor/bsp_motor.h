#ifndef __MOTOR_H
#define	__MOTOR_H


#include "stm32f10x.h"


/* ����MOTOR���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ����� */
#define AIN_GPIO_CLK 	    	RCC_APB2Periph_GPIOB	
#define AIN_GPIO_PORT    	  GPIOB		
	              	
#define AIN1_GPIO_PIN			  GPIO_Pin_8	              
#define AIN2_GPIO_PIN			  GPIO_Pin_9
	              	
#define BIN1_GPIO_PIN			  GPIO_Pin_6		     
#define BIN2_GPIO_PIN			  GPIO_Pin_7





void Motor_GPIO_Config(void);
void Motor_Control1(void);
void Motor_Control2(void);
void Motor_turn(void);
void Motor_Astern(void);
#endif /*__MOTOR_H*/
