/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_AdvanceTim.h"
#include "bsp_motor.h" 
#include "bsp_SysTick.h"
u16 USART_RX_STA=0;
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
char angle;
char FH;
char flag;
u8 i;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
			ucTemp = USART_ReceiveData(DEBUG_USARTx);
			if((USART_RX_STA&0X8000) == 0)//接收未完成
			{
				if(USART_RX_STA&0x4000)//接收到了0x0d
				{
					if(ucTemp!=0x0a)
						USART_RX_STA=0;//接收错误,重新开始
					else
						USART_RX_STA|=0x8000;	//接收完成了
				}
				else //还没收到0X0D
				{
					if(ucTemp==0x0d)//收到0X0D
						USART_RX_STA|=0x4000;
					else
					{
						USART_RX_BUF[USART_RX_STA&0X3FFF]=ucTemp ;//接受buff
						USART_RX_STA++;
						if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;//接收数据错误,重新开始接收
					}
				}
			}
	}	 
}


void GENERAL_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		if(USART_RX_STA&0x8000)
		{
			FH=USART_RX_BUF[1];
			angle=USART_RX_BUF[3];
			flag = USART_RX_BUF[5];
				
			USART_RX_STA = 0;
		}		
			if(angle == 35 && FH ==0)
			{
				TIM_SetCompare1(GENERAL_TIM5,0);
				TIM_SetCompare2(GENERAL_TIM5,30);
			}
			else if(angle == 35 && FH ==2)
			{
				TIM_SetCompare1(GENERAL_TIM5,30);
				TIM_SetCompare2(GENERAL_TIM5,0);
			}
			else
			{
				TIM_SetCompare1(GENERAL_TIM5,35);
				TIM_SetCompare2(GENERAL_TIM5,35);
			}
			
			if(flag == 0)
			{
//			Motor_Control2();
				TIM_SetCompare1(GENERAL_TIM5,0);
				TIM_SetCompare2(GENERAL_TIM5,40);
			}
			
			else if(flag == 2)
			{
//				Motor_Control3();
				TIM_SetCompare1(GENERAL_TIM5,40);
				TIM_SetCompare2(GENERAL_TIM5,0);
			}
			else if(flag == 9)//有障碍物
			{	
				TIM_SetCompare1(GENERAL_TIM5,0);
				TIM_SetCompare2(GENERAL_TIM5,0);
				SysTick_Delay_Ms(500);
				Motor_Control2();
				SysTick_Delay_Ms(500);
				Motor_Control1();
				
				TIM_SetCompare1(ADVANCE_TIM,200);
				SysTick_Delay_Ms(900);

				TIM_SetCompare1(ADVANCE_TIM,100);
				SysTick_Delay_Ms(2000);

				flag = 3;
			}
			switch(FH)
			{
				case 2:
					TIM_SetCompare1(ADVANCE_TIM,139 + angle);
				break;
				case 1:
					TIM_SetCompare1(ADVANCE_TIM,139);
				break;
				case 0:
					TIM_SetCompare1(ADVANCE_TIM,139 - angle);
				break;
			}
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
	}	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
