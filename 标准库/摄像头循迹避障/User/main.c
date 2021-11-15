#include "stm32f10x.h"
#include "bsp_motor.h" 
#include "bsp_AdvanceTim.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"

int main(void)
{	
	USART_Config();
	ADVANCE_TIM_Init();
	Motor_GPIO_Config();
	T5_Init();
	Motor_Control1();
	while(1)
	{		
	}
}




