
// TIM���߼���ʱ��-PWM�������������ʱ��Ӧ��
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"
#include "./key/bsp_key.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
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
			TIM_SetCompare1(ADVANCE_TIM,angle1); //��Ӧ0��
			TIM_SetCompare4(ADVANCE_TIM,angle2);
		}

		
	}
	
	

}

//  while(1)
//  { 
//		for(i=50;i<251;i++)
//		{
//			SysTick_Delay_Ms(30);
//			TIM_SetCompare1(ADVANCE_TIM,i); //��Ӧ0��
//		}
//		
//		for(i=250;i>49;i--)
//		{
//			SysTick_Delay_Ms(30);
//			TIM_SetCompare1(ADVANCE_TIM,i); //��Ӧ0��
//		}
//  }


