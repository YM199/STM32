/*
Bվ����������?_?��Bվ���������MDK����ʾ��ȫ����Ҫ�˽⣬�ڴ��ļ���word�ĵ����������ҵ���		
֪������������							
CSDN�������������޿���				
���㣺����֮����

�´���QQȺ��822263013
����Ȥ��ͬѧ���Ⱥ������
*/

/*
��δ�Ƶ��Ĵ���PID���������汾��ƽ��С�����ơ�
����ֻ��Ҫ��"control.c"�����޸Ļ�е��ֵ�ǶȾͿ����ˡ�
*/




#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;						//�Ƕ�
short gyrox,gyroy,gyroz;				//������--���ٶ�
short aacx,aacy,aacz;						//���ٶ�
int Encoder_Left,Encoder_Right;	//���������ݣ��ٶȣ�

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM�޷�����
int MOTO1,MOTO2;								//���װ�ر���

extern int Vertical_out,Velocity_out,Turn_out;
int main(void)	
{
	delay_init();
	NVIC_Config();
	uart1_init(115200);
	uart3_init(115200);
	delay_ms(100);
	USART3_Send_String("AT\r\n");
	delay_ms(100);
	USART3_Send_String("AT+NAMEYM\r\n");
	delay_ms(100);
	USART3_Send_String("AT+BAUD8\r\n");
	delay_ms(100);
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM1(0,7199);
  while(1)	
	{
		OLED_Float(0,0,Pitch,1);
		OLED_Float(50,50,Velocity_out,1);
//		OLED_ShowNumber(30,0,Velocity_out,3,16);//OK
	} 	
}



