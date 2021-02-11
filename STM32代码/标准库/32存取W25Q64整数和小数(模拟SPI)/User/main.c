#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "w25q64.h"
#include "spi.h"
#include "bsp_usart.h"

float data[] = {0.11,1.12,2.23,3.34,4.45,5.55};
float readdata[6]={0};

int   num[] = {1,2,3,4,5,6};
int   rnum[6] = {0};
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	u8 flag=0xcc,i=0;
	u8 rflag;
	USART_Config();
	Software_SPI_Init();
	Read_Device_ID();
	SysTick_Delay_Us(10);
	Read_JEDEC_ID();
	while(1)
	{
		Flash_Read(&rflag,SPI_FLASH_PageSize*0,1);
		if(rflag==0xcc)//������
		{
			printf("\r\n�����ݣ����ж�ȡ\r\n");
			Flash_Read((void*)readdata,SPI_FLASH_PageSize*1,sizeof(data));
			Flash_Read((void*)rnum,SPI_FLASH_PageSize*2,sizeof(num));
			for(i=0;i<6;i++)
			{
				printf("%f\r\n%d\r\n",readdata[i],rnum[i]);
			}
			printf("��ȡ��ɣ����в���,�븴λ����д��\r\n");
			FLASH_SectorErase(0);
			while(1);
		}
		else
		{
			printf("������ ��ʼд��\r\n");
			SPI_FLASH_BufferWrite(&flag,SPI_FLASH_PageSize*0,1);	
			SPI_FLASH_BufferWrite((void*)data,SPI_FLASH_PageSize*1,sizeof(data));//С��
			SPI_FLASH_BufferWrite((void*)num,SPI_FLASH_PageSize*2,sizeof(num));//����
			printf("д�����,�븴λ��ȡ\r\n");
			while(1);
		}
	}

}

/*********************************************END OF FILE**********************/
