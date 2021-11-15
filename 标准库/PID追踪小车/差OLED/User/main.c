#include "stm32f10x.h"
#include "usart_b.h"
#include "usart_a.h"
#include "usart.h"
#include "bsp_AdvanceTim.h" 
#include "bsp_motor.h"  
#include "pid.h"
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_B_Config();
	USART_A_Config();
	PID_Init();
	ADVANCE_TIM_Init();
	Motor_GPIO_Config();
	Motor_Control1();
  while(1)
	{		
		determine_interrupt();
	}	
}
/*********************************************END OF FILE**********************/
