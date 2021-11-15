#include "usart2.h"

static void USART2_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_InitTypeDefStructure;
	//TX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitTypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
	
	//RX
	GPIO_InitTypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeDefStructure.GPIO_Pin  = GPIO_Pin_3;
	
	GPIO_Init(GPIOA,&GPIO_InitTypeDefStructure);
}


static void USART2_Config(void)
{
	/*ʱ�ӳ�ʼ��������ǰ�棬*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	NVIC_InitTypeDef   NVIC_InitTypeDefStructure;

			/*�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDefStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitTypeDefStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
	NVIC_InitTypeDefStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
	NVIC_InitTypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeDefStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ʹ�ܴ��ڽ����ж�
	
	USART_InitTypeDef  USART_InitTypeDefStructure;
	USART_InitTypeDefStructure.USART_BaudRate = 115200;
	USART_InitTypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitTypeDefStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�һ��
	USART_InitTypeDefStructure.USART_Parity =USART_Parity_No;//��У��
	USART_InitTypeDefStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitTypeDefStructure.USART_WordLength = USART_WordLength_8b;//��У��λ ��λ
	
	USART_Init(USART2,&USART_InitTypeDefStructure);
	
	USART_Cmd(USART2,ENABLE);
}


/*�������ų�ʼ��������ڴ��ڳ�ʼ��֮ǰ�����߻�����쳣����0XFF*/
void USART2_Init(void)
{
	USART2_Config();
	USART2_GPIO_Config();
}

