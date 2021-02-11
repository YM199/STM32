#include "w25q64.h"

/*读取设备ID*/
void Read_Device_ID(void)
{
	u8 ID;
	
	SPI_CS_0();
	Software_SPI_Write_Read(Device_ID);
	Software_SPI_Write_Read(Dummy_Bytes);
	Software_SPI_Write_Read(Dummy_Bytes);
	Software_SPI_Write_Read(Dummy_Bytes);
	ID = Software_SPI_Write_Read(Dummy_Bytes);
	SPI_CS_1();
	printf("Device_ID：%x\r\n",ID);
}


/*读取制造商ID*/
void Read_JEDEC_ID(void)
{
	u32 Temp;
	u8 Temp0,Temp1,Temp2;
	SPI_CS_0();
	Software_SPI_Write_Read(JEDEC_ID);
	Temp0 = Software_SPI_Write_Read(Dummy_Bytes);
	Temp1 = Software_SPI_Write_Read(Dummy_Bytes);
	Temp2 = Software_SPI_Write_Read(Dummy_Bytes);
	SPI_CS_1();
	 /*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	while(Temp!=0XEF4017);
	printf("检测到W25Q64\r\n");
}


/*
   *写使能
*/
static void FLASH_WriteEnable(void)
{
	SPI_CS_0();
	Software_SPI_Write_Read(Write_Enable);
	SPI_CS_1();
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
static void SPI_FLASH_WaitForWriteEnd(void)
{
	u8 flag=0;
	SPI_CS_0();
	Software_SPI_Write_Read(ReadStatusReg1);
	flag = Software_SPI_Write_Read(Dummy_Bytes);
	while(flag&0x01)
	{
		flag = Software_SPI_Write_Read(Dummy_Bytes);
	}
	SPI_CS_1();
}

 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void FLASH_SectorErase(u32 SectorAddr)
{
	FLASH_WriteEnable();//写使能
	SPI_FLASH_WaitForWriteEnd();//等待其他操作完成才开始擦除
	SPI_CS_0();
	Software_SPI_Write_Read(Sector_Erase);//发送擦除指令
	Software_SPI_Write_Read((SectorAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((SectorAddr&0X00FF00)>>8);
	Software_SPI_Write_Read((SectorAddr&0X0000FF)>>0);
	SPI_CS_1();
	SPI_FLASH_WaitForWriteEnd();//等待擦除完毕
	printf("擦除完毕\r\n");
}
/*
 * @brief 页写入数据，调用本函数写入数据前必须先擦除扇区 
 * @para  pBuffer 待写入数据的指针
 * @para  WriteAddr 写入地址
 * @para  NumByteToWrite 数据长度，必须小于SPI_FLASH_PerWritePageSize
*/
void SPI_FLASH_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	FLASH_WriteEnable();//在写数据之前一定要进行写使能，不然会写入失败
	SPI_CS_0();
	Software_SPI_Write_Read(Page_Program);//发送页写指令
	Software_SPI_Write_Read((WriteAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((WriteAddr&0x00FF00)>>8);
	Software_SPI_Write_Read((WriteAddr&0x0000FF)>>0);
	if(NumByteToWrite>SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		printf("超出页写最大范围\r\n");
	}
	while(NumByteToWrite--)
	{
		
		Software_SPI_Write_Read(*pBuffer);
		pBuffer++;
	}
	SPI_CS_1();
	SPI_FLASH_WaitForWriteEnd();//等待写入完毕
}

 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 Addr=0,NumOfSingle=0,Num0fPage=0,count=0,temp=0;
	
	//判读地址是否页对齐
	Addr = WriteAddr % SPI_FLASH_PageSize;
	//差count个数据值，刚好可以到页对齐地址
	count = SPI_FLASH_PageSize - Addr;
	//计算要写多少页
	Num0fPage = NumByteToWrite / SPI_FLASH_PageSize;
	//计算不满一页的字节数
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	if(Addr==0)//地址页对齐
	{
		if(Num0fPage==0)//待写入的数据小于一页
		{
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else//待写入的数据大于一页
		{
			while(Num0fPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;//下一页地址
				pBuffer += SPI_FLASH_PageSize;//下一页数据
			}
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);//将余下不满一页的数据写入
		}
	}
	else//地址不对齐
	{
		if(Num0fPage==0)//待写入的数据小于一页
		{
			if(NumOfSingle>count)//当前页剩余写不下
			{
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);//先写满当前页
				WriteAddr += count;
				pBuffer += count;
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				//页剩余能够写完数据
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else//待写数据大于一页
		{
			//地址不对齐多出的count分开处理。不加入这个运算
			NumByteToWrite -= count;
			Num0fPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			//先写完count个数据，为的是让下一次要写的地址对齐
			
			//重复地址对齐的情况
			WriteAddr += count;
			pBuffer += count;
			while(Num0fPage--)//把整数页写了
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
			  pBuffer += SPI_FLASH_PageSize;
			}
			/*若有多余的不满一页的数据，把它写完*/
			if(NumOfSingle != 0)
			{
				 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}

	}
}

 /**
  * @brief   读取FLASH数据
  * @param 	 pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval  无
  */
void Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SPI_CS_0();
	//读指令
	Software_SPI_Write_Read(Read_Data);
	//读地址
	Software_SPI_Write_Read((ReadAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((ReadAddr&0x00FF00)>>8);
	Software_SPI_Write_Read((ReadAddr&0x0000FF)>>0);
	//读取数据
	while(NumByteToRead--)
	{
		*pBuffer = Software_SPI_Write_Read(Dummy_Bytes);
		pBuffer++;
	}
	SPI_CS_1();
}
