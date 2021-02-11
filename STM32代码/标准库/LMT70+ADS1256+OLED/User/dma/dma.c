#include "dma.h"
__IO uint16_t ADC_ConvertedValue;

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 复位DMA控制器
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//从外设读
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 禁止存储器到存储器的传输
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;//存储器地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度半字
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//存储器地址不递增
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );//ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设地址不增
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// 优先级：中	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

