#include "usart.h"

UART_HandleTypeDef UartHandle;

void Usart_Config(void)
{
	UartHandle.Instance = USARTx;
	UartHandle.Init.BaudRate = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UartHandle);
	//ʹ�ܽ����ж�
	__HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USARTx_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();
	USARTx_TX_GPIO_CLK_ENABLE();
	
	//TX
	GPIO_InitStruct.Pin = USARTx_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USARTx_TX_GPIO_PORT,&GPIO_InitStruct);
	
	//RX
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(USARTx_RX_GPIO_PORT,&GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(USARTx_IRQ ,0,1);//��ռ���ȼ�0�������ȼ�1
	HAL_NVIC_EnableIRQ(USARTx_IRQ);//ʹ��USART1�ж�ͨ��
}

//�����ַ���
void Usart_SendString(uint8_t *str)
{
	uint32_t k = 0;
	do
	{
		HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k),1,1000);
		k++;
	}while(*(str+k) != '\0');
}


//�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

//�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}
