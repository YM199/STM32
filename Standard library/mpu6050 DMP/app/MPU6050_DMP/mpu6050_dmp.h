#ifndef _mpu6050_dmp_H_
#define _mpu6050_dmp_H_
#include "sys.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
 
#define   MPU6050
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ӳ����8������
/* The following functions must be defined for this platform:
 * i2c_write(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char const *data)
 * i2c_read(unsigned char slave_addr, unsigned char reg_addr,
 *      unsigned char length, unsigned char *data)
 * delay_ms(unsigned long num_ms)
 * get_ms(unsigned long *count)
 * reg_int_cb(void (*cb)(void), unsigned char port, unsigned char pin)
 * labs(long x)
 * fabsf(float x)
 * min(int a, int b)
 * #define log_i(...)    
 * #define log_e(...)    
 */

#define MPU6050_IO_Init        MPU6050_IIC_IO_Init 

//����8�������Խ�dmp��
#define delay_ms    delay_ms       //delayʵ��
#define get_ms      get_ms         //�Լ�дһ��
#define i2c_write   mpu6050_write
#define i2c_read    mpu6050_read
#define labs        labs           //stm32�Դ�
#define fabs        fabsf          //stm32�Դ�
#define min(a,b)    ((a<b)?a:b)    //�Լ�дһ��
#define log_i(...)  do {} while (0)//�Լ�дһ��
#define log_e(...)  do {} while (0)//�Լ�дһ��
	
void get_ms(unsigned long *time);	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#define q30  1073741824.0f

extern float Pitch,Roll,Yaw;
extern short gyro[3], accel[3];
extern u8 mpu6050_flag;

void MPU6050_DMP_Test(void);
u8   MPU6050_DMP_Init(void);
void DEFAULT_MPU_HZ_GET(void);//10MS  100HZ����һ�£��� DEFAULT_MPU_HZ Ƶ�ʱ���һ��
 
void EXTI9_5_Init(void);

#endif
