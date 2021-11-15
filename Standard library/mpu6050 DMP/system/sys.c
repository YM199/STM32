#include "sys.h"
 
#define  printf_usartx   USART1       //���ڼ���printf

#pragma import(__use_no_semihosting)                    
struct __FILE {int handle;};          //��׼����Ҫ��֧�ֺ��� 
FILE __stdout;         
//����ʹ�ð�����ģʽ
void _sys_exit(int x){x = x;}
void _ttywrch(int ch){ch = ch;}
int fputc(int ch, FILE *f)            //�ض���fputc���� 
{      
	while((printf_usartx->SR&0X40)==0){}//ѭ������,ֱ���������   
     printf_usartx->DR = (u8) ch;      
	return ch;
}	

//stm32 ͨ�������ʼ�����������㲻ͬоƬ��ֲ
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIO_My_TypeDef mode,GPIO_My_TypeDef up_down,GPIO_My_TypeDef speed)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(power(2,((uint32_t)GPIOx-(uint32_t)GPIOA)/0x0400+2), ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;        
 
	if(mode==GPIO_AD_IN) 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	if(mode==GPIO_FK_IN && up_down==GPIO_P_NO) 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	if(mode==GPIO_FK_IN && up_down==GPIO_P_DOWN)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	if(mode==GPIO_FK_IN && up_down==GPIO_P_UP)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	if(mode==GPIO_KL_OUT)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	if(mode==GPIO_TW_OUT)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	if(mode==GPIO_KL_AF_OUT)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	if(mode==GPIO_TW_AF_OUT)		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

 	if(speed==GPIO_2MHz)
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	if(speed==GPIO_25MHz)
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if(speed==GPIO_50MHz)
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               

  GPIO_Init(GPIOx, &GPIO_InitStructure);         	
}
 
u32 power(u32 x,u32 y)
{
		u32 mul = 1;

		u32 i;
		for( i = 0;i<y;i++)
		{
				mul = mul * x;
		}
		return mul;
}

u32 log_2(u32 x)
{
		u8 i=0;
		while(1)
		{
				if(power(2,i)==x)
					return i;
				else
					i++;
		}
}
 
void assert_failed(u8* file, u32 line)
{ 
		printf("����:[�ļ�:%s] [��:%d]\r\n",file,line);  
}
