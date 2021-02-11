#ifndef _HCSR_H
#define	_HCSR_H


#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_GeneralTim.h"  
#include "bsp_usart.h"

#define Trig_GPIO_PORT    	          GPIOA			             
#define Trig_GPIO_PORT_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define Trig_GPIO_PORT_GPIO_PIN				GPIO_Pin_1			        

void Trig_GPIO_Config(void);
void Start(void);
void calculation_distance(void);
#endif /* _HCSR_H */
