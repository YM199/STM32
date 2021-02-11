#include "mpu6050_dmp.h"
#include "usart1.h"
#include "delay.h"
#include "tim_tim.h"
 
#define MPU6050_DMP_MODE  1//0:定时器查询   1:INT中断引脚读取(推荐)

void MPU6050_DMP_Test(void)
{
		MPU6050_DMP_Init();  //初始化陀螺仪
#if MPU6050_DMP_MODE==0
		TIM2_Time_Init(99,7199);//10ms	
		while(1)
		{ 
				if(mpu6050_flag==1)
				{
					mpu6050_flag=0;
					printf("%5.1f,%5.1f,%5.1f\r\n",-Pitch,Roll,-Yaw);
				}
		}
#endif
#if MPU6050_DMP_MODE==1
		EXTI9_5_Init();
		while(1)
		{ 
 
		}
#endif

}
/*
https://www.amobbs.com/thread-5580305-1-1.html

可以通过修改 mpu_run_self_test 函数下的 get_st_biases 函数里的最后一个if语句，如下：
 
只要把accel[2]数据中的标号修改成你想要的方向就好了，0是x轴，1是y轴，2是z轴，DMP默认程序写的是accel[2]，所以只能通过将芯片水平放置让z轴和G重合才能让mpu_run_self_test函数自检成功。

if (accel[1] > 0L)             //数组2修改为1 由z轴水平初始变为y轴水平初始
        accel[1] -= 65536L;
    else
        accel[1] += 65536L;   
				
run_self_test()函数内
long gyro[3], accel[3];默认没给初值，if (result == 0x7)一定成立
对好轴自检通过把if结束后的这两个值存起来，以后定义的时候直接用，这个run_self_test一定一次过
*/

static signed char gyro_orientation[9] = {-1, 0, 0,//不要改
                                           0,-1, 0,
                                           0, 0, 1};
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
float Pitch,Roll,Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
u8 mpu6050_flag=0;
 
void get_ms(unsigned long *time){}	
 
u8 MPU6050_DMP_Init(void)
{
	u16 flag=0;
	printf("MPU6050_IO_Init...\r\n");
	MPU6050_IO_Init();                                                    
	if(!mpu_init())                                                               //mpu初始化
	{	
    printf("设置需要的数据...\r\n");		
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))		                      //设置需要的传感器
		printf("为需要的数据开启fifo...\r\n");	
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	                      //设置fifo
		printf("设置采样频率=%3dHz...\r\n",DEFAULT_MPU_HZ);	
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  		                        //设置采集样率,多久更新fifo，并产生int中断
		printf("加载dmp固件...\r\n");
		if(!dmp_load_motion_driver_firmware())   	  			                          //加载dmp固件
		printf("设置陀螺仪方向...\r\n");
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))//设置陀螺仪方向
		printf("使能dma特性...\r\n");
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL))		   	 					                                //dmp_enable_feature
		printf("设置采样频率=%3dHz...\r\n",DEFAULT_MPU_HZ);
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			                            //设置采集样率,多久更新fifo，并产生int中断
		printf("自检...\r\n");
		run_self_test();		                                                        //自检
		printf("自检OK...\r\n");
		printf("使能...\r\n");
		while(mpu_set_dmp_state(1)!=0)                                              //使能
		{
			  flag++;
			  if(flag>50)
					 break;
		    delay_ms(100);
		}
		if(flag>=50)
		{
			printf("使能失败,进入while(1)...\r\n");
		  while(1)
			{
			}
			return 1;
		}
    else	
			printf("使能OK...\r\n");
	}
	printf("MPU6050_Init OK...\r\n\r\n");
	return 0;
}


void DEFAULT_MPU_HZ_GET(void)//10MS  100HZ调用一下，给 DEFAULT_MPU_HZ 频率保持一致，或者int中断引脚读取
{
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	 
	if(sensors & INV_WXYZ_QUAT )
	{
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;

		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	                                // pitch
		Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	              //yaw
		mpu6050_flag=1;
	}
}
 
void TIM2_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
		{
			  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
				DEFAULT_MPU_HZ_GET();
		}
}
 
void EXTI9_5_Init(void) 
{
		EXTI_InitTypeDef  EXTI_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
		
		GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPD; 		        
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;		 
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 
		GPIO_SetBits(GPIOB,GPIO_Pin_5);//拉高			               
 
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
		EXTI_InitStructure.EXTI_Line    = EXTI_Line5;
		EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
		EXTI_Init(&EXTI_InitStructure);	 	
		
		NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;			
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x00;		 
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			 
		NVIC_Init(&NVIC_InitStructure);  		 
}
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		  EXTI_ClearITPendingBit(EXTI_Line5);//清除标志 
		  DEFAULT_MPU_HZ_GET();
		  printf("....%5.1f,%5.1f,%5.1f\r\n",-Pitch,Roll,-Yaw);//应用时候屏蔽打印
	}
}
