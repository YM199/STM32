#include "usart1.h"


static void USART1_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_InitTypeDefStructure;
	//TX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitTypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	//RX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	
}


static void USART1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef  USART_InitTypeDefStructure;
	
	
	USART_InitTypeDefStructure.USART_BaudRate = 115200;
	USART_InitTypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitTypeDefStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�һ��
	USART_InitTypeDefStructure.USART_Parity =USART_Parity_No;//��У��
	USART_InitTypeDefStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitTypeDefStructure.USART_WordLength = USART_WordLength_8b;//��У��λ ��λ
	
	USART_Init(USART1,&USART_InitTypeDefStructure);
	
	USART_Cmd(USART1,ENABLE);
}


/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, u8 *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*�������ų�ʼ��������ڴ��ڳ�ʼ��֮ǰ�����߻�����쳣����0XFF*/
void USART1_Init(void)
{
	USART1_Config();
	USART1_GPIO_Config();
}

