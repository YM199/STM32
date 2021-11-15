#include "dma.h"
__IO uint16_t ADC_ConvertedValue;

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ��λDMA������
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�������
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ֹ�洢�����洢���Ĵ���
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;//�洢����ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ�Ȱ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//�洢����ַ������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );//ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// �����ַ����
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// ���ȼ�����	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

