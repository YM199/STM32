#include "stm32f10x.h"
#include "usart_b.h"
#include "usart_a.h"
#include "usart.h"


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
	/* ����һ���ַ��� */
	printf("��ӭʹ��Ұ��STM32������\n\n\n\n");
  while(1)
	{	
		determine_interrupt();
	}	
}
//			USART_SendData(USART1, USART_RX_BUF[0]);//�򴮿�1��������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���

/*********************************************END OF FILE**********************/
