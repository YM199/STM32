#include "mpu6050_dmp.h"
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
																		 															
void MPU6050_DMP_Test(void)
{
	MPU6050_DMP_Init();
	EXTI_Config();
}

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
		}
    else	
			printf("使能OK...\r\n");
	}
	printf("MPU6050_Init OK...\r\n\r\n");
	return 0;
}

//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
//roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
u8 mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//计算得到俯仰角/横滚角/航向角
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=Sensors_I2C_ReadRegister(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=Sensors_I2C_ReadRegister(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
