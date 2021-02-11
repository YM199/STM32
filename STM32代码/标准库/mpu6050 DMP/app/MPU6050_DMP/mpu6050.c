#include "mpu6050.h"


void MPU6050_Test(void)
{
		MPU6050_Init();  //��ʼ��������
 
		while(1)
		{ 
				MPU6050_Data();
		}
}

u8 mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf)//����ֵ 0�����ɹ�  -1����ʧ��
{ 
		 unsigned char i;
     addr=addr<<1;                     //ע��dma���ַ���������һλ����Ҫ��λ
		 MPU6050_IIC_Start();              //��������
		 MPU6050_IIC_Send_Byte(addr);      //����������ַ           
		 MPU6050_IIC_Send_Byte(reg);       //���������ӵ�ַ

		 for(i=0;i<len;i++)            
			 MPU6050_IIC_Send_Byte(*buf++);  //��������
		 MPU6050_IIC_Stop();               //��������

		 return 0;
}

u8 mpu6050_read(u8 addr, u8 reg, u8 len, u8 *buf)//����ֵ 0�����ɹ�  -1����ʧ��
{
		 unsigned char i;
     addr=addr<<1;                     //ע��dma���ַ���������һλ����Ҫ��λ
		 MPU6050_IIC_Start();              //��������           
		 MPU6050_IIC_Send_Byte(addr);      //����������ַ            
		 MPU6050_IIC_Send_Byte(reg);       //���������ӵ�ַ

		 MPU6050_IIC_Start();              //������������
		 MPU6050_IIC_Send_Byte(addr+1);
		 for(i=0;i<len-1;i++)  
			 *buf++=MPU6050_IIC_Read_Byte(0);//��������
		 *buf=MPU6050_IIC_Read_Byte(1);
		 MPU6050_IIC_Stop();               //��������
	
		 return 0;
}

void mpu6050_write_reg(u8 reg, u8 dat)
{
   mpu6050_write(SlaveAddress,reg,1,&dat);
}

u8   mpu6050_read_reg (u8 reg)
{
	 u8 dat;
   mpu6050_read(SlaveAddress,reg,1,&dat);
	 return dat;
}

void MPU6050_Init(void)
{
    MPU6050_IIC_IO_Init(); 
 
    printf("MPU6050_ID=%x\r\n",mpu6050_read_reg(WHO_AM_I));
	
    mpu6050_write_reg(PWR_MGMT_1,  0x00);	//�������״̬
    mpu6050_write_reg(SMPLRT_DIV,  0x07); //�����ǲ�����125Hz
    mpu6050_write_reg(CONFIG,      0x06); //��ͨ�˲�Ƶ��5Hz
	  mpu6050_write_reg(GYRO_CONFIG, 0x18); //�������Լ켰������Χ:���Լ죬2000deg/s
	  mpu6050_write_reg(ACCEL_CONFIG,0x01); //���ټ��Լ졢������Χ����ͨ�˲�Ƶ��:���Լ죬2G��5Hz
}
 
void MPU6050_Data(void)
{
	u8 i;
	s16 ax,ay,az,gx,gy,gz;
	s8 data[14] ;
	for(i=0;i<14;i++) 
	{
	    data[i]=mpu6050_read_reg(0x3B+i);
	}
	ax=(data[ 0]<<8)+data[ 1];
	ay=(data[ 2]<<8)+data[ 3];
  az=(data[ 4]<<8)+data[ 5];
  gx=(data[ 8]<<8)+data[ 9];
  gy=(data[10]<<8)+data[11];
  gz=(data[12]<<8)+data[13];
  i++;
  printf("%6d,%6d,%6d,%6d,%6d,%6d\r\n",ax,ay,az,gx,gy,gz);

}