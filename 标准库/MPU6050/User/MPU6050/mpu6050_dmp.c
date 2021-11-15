#include "mpu6050_dmp.h"
/*
https://www.amobbs.com/thread-5580305-1-1.html

����ͨ���޸� mpu_run_self_test �����µ� get_st_biases ����������һ��if��䣬���£�
 
ֻҪ��accel[2]�����еı���޸ĳ�����Ҫ�ķ���ͺ��ˣ�0��x�ᣬ1��y�ᣬ2��z�ᣬDMPĬ�ϳ���д����accel[2]������ֻ��ͨ����оƬˮƽ������z���G�غϲ�����mpu_run_self_test�����Լ�ɹ���

if (accel[1] > 0L)             //����2�޸�Ϊ1 ��z��ˮƽ��ʼ��Ϊy��ˮƽ��ʼ
        accel[1] -= 65536L;
    else
        accel[1] += 65536L;   
				
run_self_test()������
long gyro[3], accel[3];Ĭ��û����ֵ��if (result == 0x7)һ������
�Ժ����Լ�ͨ����if�������������ֵ���������Ժ����ʱ��ֱ���ã����run_self_testһ��һ�ι�
*/
static signed char gyro_orientation[9] = {-1, 0, 0,//��Ҫ��
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
	if(!mpu_init())                                                               //mpu��ʼ��
	{	
    printf("������Ҫ������...\r\n");		
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))		                      //������Ҫ�Ĵ�����
		printf("Ϊ��Ҫ�����ݿ���fifo...\r\n");	
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	                      //����fifo
		printf("���ò���Ƶ��=%3dHz...\r\n",DEFAULT_MPU_HZ);	
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  		                        //���òɼ�����,��ø���fifo��������int�ж�
		printf("����dmp�̼�...\r\n");
		if(!dmp_load_motion_driver_firmware())   	  			                          //����dmp�̼�
		printf("���������Ƿ���...\r\n");
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))//���������Ƿ���
		printf("ʹ��dma����...\r\n");
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL))		   	 					                                //dmp_enable_feature
		printf("���ò���Ƶ��=%3dHz...\r\n",DEFAULT_MPU_HZ);
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			                            //���òɼ�����,��ø���fifo��������int�ж�
		printf("�Լ�...\r\n");
		run_self_test();		                                                        //�Լ�
		printf("�Լ�OK...\r\n");
		printf("ʹ��...\r\n");
		while(mpu_set_dmp_state(1)!=0)                                              //ʹ��
		{
			  flag++;
			  if(flag>50)
					 break;
		    delay_ms(100);
		}
		if(flag>=50)
		{
			printf("ʹ��ʧ��,����while(1)...\r\n");
		  while(1)
			{
			}
		}
    else	
			printf("ʹ��OK...\r\n");
	}
	printf("MPU6050_Init OK...\r\n\r\n");
	return 0;
}

//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
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
		q0 = quat[0] / q30;	//q30��ʽת��Ϊ������
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//����õ�������/�����/�����
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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


//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
