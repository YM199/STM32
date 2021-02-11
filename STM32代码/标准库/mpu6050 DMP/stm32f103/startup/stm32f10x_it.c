#include "stm32f10x_it.h"
 
///////////////////////////////////////////////////////////////////////////////
//系统内部中断
void NMI_Handler        (void){           }//内部中断，硬件自动清除标志位
void HardFault_Handler  (void){while (1){}}//内部中断，硬件自动清除标志位
void MemManage_Handler  (void){while (1){}}//内部中断，硬件自动清除标志位
void BusFault_Handler   (void){while (1){}}//内部中断，硬件自动清除标志位
void UsageFault_Handler (void){while (1){}}//内部中断，硬件自动清除标志位
void DebugMon_Handler   (void){           }
void PendSV_Handler     (void){           }//内部中断，硬件自动清除标志位
void SVC_Handler        (void){           }//内部中断，硬件自动清除标志位
/////////////////////////////////////////////////////////////////////////////// 

