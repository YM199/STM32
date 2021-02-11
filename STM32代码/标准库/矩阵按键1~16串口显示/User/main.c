#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "bsp_key.h"
#include "delay.h"
#include "bsp_usart.h"

uint8_t A = 0;


int main(void)
{
	USART_Config();
	Key_Config();
	delayInit();
  while(1)
	{
		A = KEY_Scan(0);
		if(A != 0)
		{
			USART_SendData(DEBUG_USARTx,A);
			printf("\r\n%d\r\n",A);
      while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TC));
		}
	}
}

