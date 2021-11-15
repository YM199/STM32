#ifndef __MOTOR_H
#define	__MOTOR_H


#include "stm32f10x.h"


/* 定义MOTOR连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的引脚 */
#define AIN_GPIO_CLK 	    	RCC_APB2Periph_GPIOB	
#define AIN_GPIO_PORT    	  GPIOB		
	              	
#define AIN1_GPIO_PIN			  GPIO_Pin_8	              
#define AIN2_GPIO_PIN			  GPIO_Pin_9
	              	
#define BIN1_GPIO_PIN			  GPIO_Pin_6		     
#define BIN2_GPIO_PIN			  GPIO_Pin_7

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)



void Motor_GPIO_Config(void);
void Motor_Control1(void);
void Motor_Control2(void);
#endif /*__MOTOR_H*/
