/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : delay.c
*描述           : STM32103 延时初始化函数
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.4.2
*备注           : 1.使用了SysTick计数模式对延迟进行管理
                  2.等待延时
*********************************************************************/
#include "delay.h"
static u8  fac_us=0;	
static u16 fac_ms=0;
/********************************************************************
*函数名称       : delayInit
*功能说明       : 延时函数初始化函数
*参数说明       :
                  [IN]
									     无					 
                  [OUT]
                        无
*函数返回				: 无
*修改时间				:2017.4.2
*备注						:延时函数使用了systick定时器，初始化了时钟，和延时变量
*作者           :武旭东
*********************************************************************/
void delayInit(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	/*选择外部时钟，HCLK/8*/
	fac_us=SystemCoreClock/8000000;/*为系统时钟的1/8*/			
	fac_ms=(u16)fac_us*1000;
}
/********************************************************************
*函数名称       : delayUs
*功能说明       : 1us延时函数
*参数说明       :
                  [IN]
									     无					 
                  [OUT]
                        无
*函数返回				: 无
*修改时间				:2017.4.2
*备注						:1us延时
*作者           :武旭东
*********************************************************************/
void delayUs(u32 nus)
 { 
	u32 temp;	
	SysTick->LOAD=nus*fac_us; 	/*时间加载*/					 
	SysTick->VAL=0x00;      /*清空计数器*/
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;/*开始计数*/ 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		/*等待时间到达*/
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;/*关闭技术器*/
	SysTick->VAL =0X00;     /*清空计数器*/
} 
 /********************************************************************
*函数名称       : delayMs
*功能说明       : 1ms延时函数
*参数说明       :
                  [IN]
									     无					 
                  [OUT]
                        无
*函数返回				: 无
*修改时间				:2017.4.2
*备注						:1ms延时 ,在72M时钟下，最大延时为1864
*作者           :武旭东
*********************************************************************/
void delayMs(u16 nms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				
	SysTick->VAL =0x00;						
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;       			
} 
