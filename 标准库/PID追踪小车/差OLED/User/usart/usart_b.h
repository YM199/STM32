#ifndef __USART_B_H
#define	__USART_B_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	

//// 串口1-USART1
//#define  USART_B                   USART1
//#define  USART_B_CLK                RCC_APB2Periph_USART1
//#define  USART_B_APBxClkCmd         RCC_APB2PeriphClockCmd
//#define  USART_B_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  USART_B_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  USART_B_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  USART_B_TX_GPIO_PORT       GPIOA   
//#define  USART_B_TX_GPIO_PIN        GPIO_Pin_9
//#define  USART_B_RX_GPIO_PORT       GPIOA
//#define  USART_B_RX_GPIO_PIN        GPIO_Pin_10

//#define  USART_B_IRQ                USART1_IRQn
//#define  USART_B_IRQHandler         USART1_IRQHandler


// 串口2-USART2
#define  USART_B                   USART2
#define  USART_B_CLK                RCC_APB1Periph_USART2
#define  USART_B_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART_B_BAUDRATE           115200

//// USART GPIO 引脚宏定义
#define  USART_B_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART_B_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART_B_TX_GPIO_PORT       GPIOA   
#define  USART_B_TX_GPIO_PIN        GPIO_Pin_2
#define  USART_B_RX_GPIO_PORT       GPIOA
#define  USART_B_RX_GPIO_PIN        GPIO_Pin_3

#define  USART_B_IRQ                USART2_IRQn
#define  USART_B_IRQHandler         USART2_IRQHandler

// 串口3-USART3
//#define  USART_B                   USART3
//#define  USART_B_CLK                RCC_APB1Periph_USART3
//#define  USART_B_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  USART_B_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  USART_B_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  USART_B_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  USART_B_TX_GPIO_PORT       GPIOB   
//#define  USART_B_TX_GPIO_PIN        GPIO_Pin_10
//#define  USART_B_RX_GPIO_PORT       GPIOB
//#define  USART_B_RX_GPIO_PIN        GPIO_Pin_11

//#define  USART_B_IRQ                USART3_IRQn
//#define  USART_B_IRQHandler         USART3_IRQHandler

// 串口4-UART4
//#define  USART_B                   UART4
//#define  USART_B_CLK                RCC_APB1Periph_UART4
//#define  USART_B_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  USART_B_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  USART_B_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  USART_B_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  USART_B_TX_GPIO_PORT       GPIOC   
//#define  USART_B_TX_GPIO_PIN        GPIO_Pin_10
//#define  USART_B_RX_GPIO_PORT       GPIOC
//#define  USART_B_RX_GPIO_PIN        GPIO_Pin_11

//#define  USART_B_IRQ                UART4_IRQn
//#define  USART_B_IRQHandler         UART4_IRQHandler


// 串口5-UART5
//#define  USART_B                   UART5
//#define  USART_B_CLK                RCC_APB1Periph_UART5
//#define  USART_B_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  USART_B_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  USART_B_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  USART_B_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  USART_B_TX_GPIO_PORT       GPIOC   
//#define  USART_B_TX_GPIO_PIN        GPIO_Pin_12
//#define  USART_B_RX_GPIO_PORT       GPIOD
//#define  USART_B_RX_GPIO_PIN        GPIO_Pin_2

//#define  USART_B_IRQ                UART5_IRQn
//#define  USART_B_IRQHandler         UART5_IRQHandler


void USART_B_Config(void);

#endif /* __USART_B_H */
