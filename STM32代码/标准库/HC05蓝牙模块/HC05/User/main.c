#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "usart1.h"
#include "usart2.h"
#include "HC05.h"
#include "bsp_SysTick.h"
/*
   USART2����ָ���������USART2������Ӧ��Ȼ����Ӧͨ��USART1���͸�����
*/



int main(void)
{
	USART1_Init();
	USART2_Init();
	HC05_Init();

	while(1);

}


