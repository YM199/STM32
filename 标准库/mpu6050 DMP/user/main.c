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

 
#define MPU6050_DMP_OPEN  1  //是否开启dmp 0:关闭 1:开启  不开只能读原始数据没加处理


//串口几用printf在sys.c中设置，默认串口1可用printf 
int main()
{	
    u64 xitong_haomiao_printf_old;

	  delay_init();
 
		USART1_Init(115200);
//	  USART2_Init(115200);
//	  USART3_Init(115200);
 
#if MPU6050_DMP_OPEN==0//只用mpu6050.c mpu6050.h mpu6050_io_iic.c mpu6050_io_iic.h 四个文件就能实现只读原始数据
	  MPU6050_Test();
#endif
#if MPU6050_DMP_OPEN==1 //全部文件都用可以用这个dmp
	  MPU6050_DMP_Test();
#endif
	

	  while(1)
		{
		}
//以下是发帖模板这个例子没用		
		
//	  SoftTimer_Init();
//    Multi_Button_Init();
//	  Key_Scan_Init();
	
		while(1)
		{
					if(USART1_RX_STA&0x8000)
					{		
						  USART1_printf("USART1_Read%5d个(含\\0):%s\r\n",(USART1_RX_STA&0x7fff),USART1_RX_BUF);
							USART1_RX_STA=0;
					}
					if(USART2_RX_STA&0x8000)
					{		
						  USART2_printf("USART2_Read%5d个(含\\0):%s\r\n",(USART2_RX_STA&0x7fff),USART2_RX_BUF);
							USART2_RX_STA=0;
					}
					if(USART3_RX_STA&0x8000)
					{		
						  USART3_printf("USART3_Read%5d个(含\\0):%s\r\n",(USART3_RX_STA&0x7fff),USART3_RX_BUF);
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

