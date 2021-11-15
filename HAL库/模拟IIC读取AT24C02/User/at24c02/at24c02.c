#include "at24c02.h"

void AT24C02_ByteWrite(uint8_t addr,uint8_t data)
{
	IIC_Start();
	IIC_Write_Byte(dev_addr | write_bit);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	IIC_Write_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
	Delay_ms(5);
}

void Page_Write(uint8_t addr,uint8_t * data,uint8_t size)
{
	uint8_t i;
	IIC_Start();
	IIC_Write_Byte(dev_addr | write_bit);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	for(i=0;i<size;i++)
	{
		IIC_Write_Byte(*(data++));
		IIC_Wait_Ack();	
	}
	IIC_Stop();
	Delay_ms(5);
}

uint8_t CurAddr_Read(void)
{
	uint8_t data;
	IIC_Start();
	IIC_Write_Byte( dev_addr | read_bit );
	IIC_Wait_Ack();
	
	data = IIC_Byte_Read();
	IIC_No_Ack();
	IIC_Stop();
	
	return data;
}

uint8_t Ran_Read(uint8_t addr)
{
	uint8_t data;
	IIC_Start();
	IIC_Write_Byte(dev_addr | write_bit);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Write_Byte( dev_addr | read_bit );
	IIC_Wait_Ack();
	data = IIC_Byte_Read();
	IIC_No_Ack();
	IIC_Stop();
	
	return data;
}

void Seq_Read(uint8_t addr,uint8_t *data,uint8_t size)
{
	uint8_t i;
	IIC_Start();
	IIC_Write_Byte(dev_addr | write_bit);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Write_Byte( dev_addr | read_bit );
	IIC_Wait_Ack();
	for(i=0;i<size;i++)
	{
		(*data) = IIC_Byte_Read();
		data ++;
		IIC_Ack();
	}
	IIC_No_Ack();
	IIC_Stop();
}

