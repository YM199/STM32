#include "w25q64.h"

/*��ȡ�豸ID*/
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
	printf("Device_ID��%x\r\n",ID);
}


/*��ȡ������ID*/
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
	 /*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	while(Temp!=0XEF4017);
	printf("��⵽W25Q64\r\n");
}


/*
   *дʹ��
*/
static void FLASH_WriteEnable(void)
{
	SPI_CS_0();
	Software_SPI_Write_Read(Write_Enable);
	SPI_CS_1();
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
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
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void FLASH_SectorErase(u32 SectorAddr)
{
	FLASH_WriteEnable();//дʹ��
	SPI_FLASH_WaitForWriteEnd();//�ȴ�����������ɲſ�ʼ����
	SPI_CS_0();
	Software_SPI_Write_Read(Sector_Erase);//���Ͳ���ָ��
	Software_SPI_Write_Read((SectorAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((SectorAddr&0X00FF00)>>8);
	Software_SPI_Write_Read((SectorAddr&0X0000FF)>>0);
	SPI_CS_1();
	SPI_FLASH_WaitForWriteEnd();//�ȴ��������
	printf("�������\r\n");
}
/*
 * @brief ҳд�����ݣ����ñ�����д������ǰ�����Ȳ������� 
 * @para  pBuffer ��д�����ݵ�ָ��
 * @para  WriteAddr д���ַ
 * @para  NumByteToWrite ���ݳ��ȣ�����С��SPI_FLASH_PerWritePageSize
*/
void SPI_FLASH_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	FLASH_WriteEnable();//��д����֮ǰһ��Ҫ����дʹ�ܣ���Ȼ��д��ʧ��
	SPI_CS_0();
	Software_SPI_Write_Read(Page_Program);//����ҳдָ��
	Software_SPI_Write_Read((WriteAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((WriteAddr&0x00FF00)>>8);
	Software_SPI_Write_Read((WriteAddr&0x0000FF)>>0);
	if(NumByteToWrite>SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		printf("����ҳд���Χ\r\n");
	}
	while(NumByteToWrite--)
	{
		
		Software_SPI_Write_Read(*pBuffer);
		pBuffer++;
	}
	SPI_CS_1();
	SPI_FLASH_WaitForWriteEnd();//�ȴ�д�����
}

 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 Addr=0,NumOfSingle=0,Num0fPage=0,count=0,temp=0;
	
	//�ж���ַ�Ƿ�ҳ����
	Addr = WriteAddr % SPI_FLASH_PageSize;
	//��count������ֵ���պÿ��Ե�ҳ�����ַ
	count = SPI_FLASH_PageSize - Addr;
	//����Ҫд����ҳ
	Num0fPage = NumByteToWrite / SPI_FLASH_PageSize;
	//���㲻��һҳ���ֽ���
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	if(Addr==0)//��ַҳ����
	{
		if(Num0fPage==0)//��д�������С��һҳ
		{
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else//��д������ݴ���һҳ
		{
			while(Num0fPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;//��һҳ��ַ
				pBuffer += SPI_FLASH_PageSize;//��һҳ����
			}
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);//�����²���һҳ������д��
		}
	}
	else//��ַ������
	{
		if(Num0fPage==0)//��д�������С��һҳ
		{
			if(NumOfSingle>count)//��ǰҳʣ��д����
			{
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);//��д����ǰҳ
				WriteAddr += count;
				pBuffer += count;
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				//ҳʣ���ܹ�д������
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else//��д���ݴ���һҳ
		{
			//��ַ����������count�ֿ������������������
			NumByteToWrite -= count;
			Num0fPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			//��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ����
			
			//�ظ���ַ��������
			WriteAddr += count;
			pBuffer += count;
			while(Num0fPage--)//������ҳд��
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
			  pBuffer += SPI_FLASH_PageSize;
			}
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
			if(NumOfSingle != 0)
			{
				 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}

	}
}

 /**
  * @brief   ��ȡFLASH����
  * @param 	 pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval  ��
  */
void Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SPI_CS_0();
	//��ָ��
	Software_SPI_Write_Read(Read_Data);
	//����ַ
	Software_SPI_Write_Read((ReadAddr&0XFF0000)>>16);
	Software_SPI_Write_Read((ReadAddr&0x00FF00)>>8);
	Software_SPI_Write_Read((ReadAddr&0x0000FF)>>0);
	//��ȡ����
	while(NumByteToRead--)
	{
		*pBuffer = Software_SPI_Write_Read(Dummy_Bytes);
		pBuffer++;
	}
	SPI_CS_1();
}
