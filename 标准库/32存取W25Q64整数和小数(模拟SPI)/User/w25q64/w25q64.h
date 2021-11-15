#ifndef _W25Q64_H
#define _W25Q64_H

#include "stm32f10x.h"
#include "spi.h"
#include "bsp_usart.h"
#define SPI_FLASH_PageSize 							256
#define SPI_FLASH_PerWritePageSize      256

#define    Device_ID     				0XAB
#define    JEDEC_ID   	 				0X9F
#define    Dummy_Bytes   				0XFF
#define    Read_Data     				0X03
#define		 Write_Enable  				0X06
#define 	 ReadStatusReg1		    0x05
#define    Sector_Erase         0x20
#define    Page_Program         0x02
void Read_Device_ID(void);
void Read_JEDEC_ID(void);
void FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
#endif 

