#ifndef _mpu6050_dmp_H_
#define _mpu6050_dmp_H_
#include "sys.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
 
#define   MPU6050
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//映射这8个函数
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

//下面8个函数对接dmp库
#define delay_ms    delay_ms       //delay实现
#define get_ms      get_ms         //自己写一个
#define i2c_write   mpu6050_write
#define i2c_read    mpu6050_read
#define labs        labs           //stm32自带
#define fabs        fabsf          //stm32自带
#define min(a,b)    ((a<b)?a:b)    //自己写一个
#define log_i(...)  do {} while (0)//自己写一个
#define log_e(...)  do {} while (0)//自己写一个
	
void get_ms(unsigned long *time);	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
#define q30  1073741824.0f

extern float Pitch,Roll,Yaw;
extern short gyro[3], accel[3];
extern u8 mpu6050_flag;

void MPU6050_DMP_Test(void);
u8   MPU6050_DMP_Init(void);
void DEFAULT_MPU_HZ_GET(void);//10MS  100HZ调用一下，给 DEFAULT_MPU_HZ 频率保持一致
 
void EXTI9_5_Init(void);

#endif
