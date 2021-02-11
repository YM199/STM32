#include "stm32f10x.h"
#include "usart_b.h"
#include "usart_a.h"
#include "usart.h"


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_B_Config();
	USART_A_Config();
	/* 发送一个字符串 */
	printf("欢迎使用野火STM32开发板\n\n\n\n");
  while(1)
	{	
		determine_interrupt();
	}	
}
//			USART_SendData(USART1, USART_RX_BUF[0]);//向串口1发送数据
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束

/*********************************************END OF FILE**********************/
