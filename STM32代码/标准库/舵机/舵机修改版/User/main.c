
// TIM—高级定时器-PWM互补输出带死区时间应用
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"
#include "./key/bsp_key.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */

	extern uint16_t angle2;
	extern uint16_t angle1;
	uint16_t JD1;
	u16 i = 50;

int main(void)
{	
	USART_Config();
	Key_Config();
	ADVANCE_TIM_Init();
	while(1)
	{
	  JD1 = Key();
		if(angle2!=0)
		{
			TIM_SetCompare1(ADVANCE_TIM,angle1); //对应0度
			TIM_SetCompare4(ADVANCE_TIM,angle2);
		}

		
	}
	
	

}

//  while(1)
//  { 
//		for(i=50;i<251;i++)
//		{
//			SysTick_Delay_Ms(30);
//			TIM_SetCompare1(ADVANCE_TIM,i); //对应0度
//		}
//		
//		for(i=250;i>49;i--)
//		{
//			SysTick_Delay_Ms(30);
//			TIM_SetCompare1(ADVANCE_TIM,i); //对应0度
//		}
//  }


