// TIM—通用-捕获-测量脉宽 应用
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h"  
#include "hc_sr04.h" 
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{


	/* 串口初始化 */
	USART_Config();
	/* 定时器初始化 */
	GENERAL_TIM_Init();
	Trig_GPIO_Config();
	
	while ( 1 )
	{
		Start();
		SysTick_Delay_Ms(1000);
	}
}
/*********************************************END OF FILE**********************/
