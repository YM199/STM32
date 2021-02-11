#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "mpu6050_dmp.h"

int main(void)
{
  USART_Config();
	SysTick_Init();
	MPU6050_DMP_Test();
	while(1);
}

