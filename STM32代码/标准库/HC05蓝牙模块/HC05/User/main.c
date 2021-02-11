#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include "usart2.h"
#include "HC05.h"
#include "bsp_SysTick.h"
/*
   USART2发送指令给蓝牙，USART2接收响应。然后响应通过USART1发送给电脑
*/



int main(void)
{
	USART1_Init();
	USART2_Init();
	HC05_Init();

	while(1);

}


