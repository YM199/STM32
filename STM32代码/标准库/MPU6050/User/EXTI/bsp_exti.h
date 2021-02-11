#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


//Òý½Å¶¨Òå
#define MPU_INT_GPIO_PORT         GPIOA
#define MPU_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define MPU_INT_GPIO_PIN          GPIO_Pin_2
#define MPU_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define MPU_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define MPU_INT_EXTI_LINE         EXTI_Line2
#define MPU_INT_EXTI_IRQ          EXTI2_IRQn

#define MPU_IRQHandler            EXTI2_IRQHandler




void EXTI_Config(void);


#endif /* __EXTI_H */
