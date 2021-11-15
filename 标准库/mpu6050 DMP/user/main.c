#include "sys.h"
#include "delay.h"
#include "usart1.h"	
#include "usart2.h"
#include "usart3.h"
#include "key.h"
#include "soft_timer.h"
#include "multi_button.h"
#include "mpu6050.h"
#include "mpu6050_dmp.h"

 
#define MPU6050_DMP_OPEN  1  //�Ƿ���dmp 0:�ر� 1:����  ����ֻ�ܶ�ԭʼ����û�Ӵ���


//���ڼ���printf��sys.c�����ã�Ĭ�ϴ���1����printf 
int main()
{	
    u64 xitong_haomiao_printf_old;

	  delay_init();
 
		USART1_Init(115200);
//	  USART2_Init(115200);
//	  USART3_Init(115200);
 
#if MPU6050_DMP_OPEN==0//ֻ��mpu6050.c mpu6050.h mpu6050_io_iic.c mpu6050_io_iic.h �ĸ��ļ�����ʵ��ֻ��ԭʼ����
	  MPU6050_Test();
#endif
#if MPU6050_DMP_OPEN==1 //ȫ���ļ����ÿ��������dmp
	  MPU6050_DMP_Test();
#endif
	

	  while(1)
		{
		}
//�����Ƿ���ģ���������û��		
		
//	  SoftTimer_Init();
//    Multi_Button_Init();
//	  Key_Scan_Init();
	
		while(1)
		{
					if(USART1_RX_STA&0x8000)
					{		
						  USART1_printf("USART1_Read%5d��(��\\0):%s\r\n",(USART1_RX_STA&0x7fff),USART1_RX_BUF);
							USART1_RX_STA=0;
					}
					if(USART2_RX_STA&0x8000)
					{		
						  USART2_printf("USART2_Read%5d��(��\\0):%s\r\n",(USART2_RX_STA&0x7fff),USART2_RX_BUF);
							USART2_RX_STA=0;
					}
					if(USART3_RX_STA&0x8000)
					{		
						  USART3_printf("USART3_Read%5d��(��\\0):%s\r\n",(USART3_RX_STA&0x7fff),USART3_RX_BUF);
							USART3_RX_STA=0;
					}
					if(xitong_haomiao-xitong_haomiao_printf_old>=1000)
					{
							xitong_haomiao_printf_old=xitong_haomiao;
							printf       ("USART1_ms=%llu\r\n",xitong_haomiao/1000);
							USART2_printf("USART2_ms=%llu\r\n",xitong_haomiao/1000);
						  USART3_printf("USART3_ms=%llu\r\n",xitong_haomiao/1000);
					}
					if(Key!=KEY_NO_PRES)
					{
							if(Key==KEY0_PRES)
							{
									printf("key0\r\n");
							}
							Key=KEY_NO_PRES;
					}
		}
}

