#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"
#include "bsp_SysTick.h"
#include "stdio.h"
#include "stdlib.h"
#include "pid.h"
#include "usart1.h"
#include "usart2.h"
#include "bsp_motor.h"  
#include "ultrasonic.h"
#include "bsp_TiMbase.h" 
u8 num;
u16 i = 50;
extern u8 flag; 
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */

int main(void)
{	
	USART1_Init();
	UltrasonicWave_Init();
	USART2_Init();
	PID_Init();
	ADVANCE_TIM_Init();
	BASIC_TIM_Init();
	Motor_GPIO_Config();
	Motor_Control1();
	while(1)
	{
		
		if(USART_RX_STA&0x8000)
		{		
				   
			num=atoi(USART_RX_BUF);
			claer();
			pid_Calc(num);
			
			USART_RX_STA=0;
		}

	}
}


