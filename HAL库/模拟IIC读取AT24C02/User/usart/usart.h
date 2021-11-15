#ifndef _USART_H
#define _USART_H

#include "stm32f1xx.h"
#include <stdio.h>

#define USARTx     												 USART1
#define USARTx_CLK_ENABLE()  							 __HAL_RCC_USART1_CLK_ENABLE()

#define USARTx_RX_GPIO_PORT                GPIOA
#define USARTx_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_RX_PIN                      GPIO_PIN_10

#define USARTx_TX_GPIO_PORT                GPIOA
#define USARTx_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_PIN                      GPIO_PIN_9

#define USARTx_IRQ                 		    USART1_IRQn
#define USARTx_IRQHandler                 USART1_IRQHandler

extern UART_HandleTypeDef UartHandle;

void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void Usart_Config(void);
#endif

