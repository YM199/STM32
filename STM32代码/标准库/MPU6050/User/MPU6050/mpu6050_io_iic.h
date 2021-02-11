#ifndef _MPU6050_IO_IIC_
#define _MPU6050_IO_IIC_

#include "stm32f10x.h"

#define EEPROM_I2C_WR	0		/* 写控制bit */
#define EEPROM_I2C_RD	1		/* 读控制bit */

//如果移植程序时只要改一下地方就行了
#define MPU6050_IIC_GPIO                   GPIOA
#define MPU6050_IIC_SCL_Pin                GPIO_Pin_0	         //PA1
#define MPU6050_IIC_SDA_Pin                GPIO_Pin_1	         //PA0
#define MPU6050_IIC_APBx                   RCC_APB2Periph_GPIOB
#define EEPROM_I2C_SCL_1()  GPIO_SetBits(MPU6050_IIC_GPIO, MPU6050_IIC_SCL_Pin)		/* SCL = 1 */
#define EEPROM_I2C_SCL_0()  GPIO_ResetBits(MPU6050_IIC_GPIO, MPU6050_IIC_SCL_Pin)		/* SCL = 0 */

#define EEPROM_I2C_SDA_READ()  GPIO_ReadInputDataBit(MPU6050_IIC_GPIO, MPU6050_IIC_SDA_Pin)	/* 读SDA口线状态 */
#define EEPROM_I2C_SDA_1()  GPIO_SetBits(MPU6050_IIC_GPIO, MPU6050_IIC_SDA_Pin)		/* SDA = 1 */
#define EEPROM_I2C_SDA_0()  GPIO_ResetBits(MPU6050_IIC_GPIO, MPU6050_IIC_SDA_Pin)		/* SDA = 0 */
		
void i2c_Start(void);
void i2c_Stop(void);
void MPU6050_IIC_IO_Init(void);
void MPU6050_IIC_SDA_IN(void);
void MPU6050_IIC_SDA_OUT(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
u8 Sensors_I2C_WriteRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char const *data);
u8 Sensors_I2C_ReadRegister(unsigned char slave_addr, unsigned char reg_addr,unsigned char length, unsigned char *data);
#endif

