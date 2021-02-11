#include "usart2.h"
u16 USART_RX_STA=0;
char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���200���ֽ�
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
	NVIC_InitTypeDefStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	NVIC_InitTypeDefStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
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
	USART2_GPIO_Config();
	USART2_Config();
	
}

void compare(u8 ucTemp)
{
	
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(ucTemp!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(ucTemp==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=ucTemp ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
}

void claer(void)
{
	int i;
	for(i=0;i<USART_REC_LEN;i++)
	USART_RX_BUF[i] = 0;
}

