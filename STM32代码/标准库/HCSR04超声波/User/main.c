// TIM��ͨ��-����-�������� Ӧ��
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h"  
#include "hc_sr04.h" 
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{


	/* ���ڳ�ʼ�� */
	USART_Config();
	/* ��ʱ����ʼ�� */
	GENERAL_TIM_Init();
	Trig_GPIO_Config();
	
	while ( 1 )
	{
		Start();
		SysTick_Delay_Ms(1000);
	}
}
/*********************************************END OF FILE**********************/
