#include "delay.h"
                                   // ms    s    h  day year
volatile u64 xitong_haomiao;       //2^64/1000/3600/24/365=584942417年会复位
//USART1_printf("xitong_haomiao=%llu\r\n",xitong_haomiao);
volatile u64 xitong_haomiao_old=0; 	   
	   
static   u8  fac_us=0;					   //us延时倍乘数	
 
void delay_init()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //1、打开复用时钟：
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//2、关闭jtag 开启sw
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	 //选择外部时钟  HCLK/8  9M 计数器减1为1/9000000秒
	fac_us=SystemCoreClock/8000000;				                 //9  
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	           //开启SYSTICK中断
	SysTick->LOAD=fac_us*1000-1; 						               //每1/1000s中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	           //开启SYSTICK    
}	

//nus:0~2^32/fac_us=477218588.444us=477s   								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							  //需要的节拍数	  		 
	tcnt=0;
	told=SysTick->VAL;        			  //刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				  //时间超过/等于要延迟的时间,则退出.
		}  
	}	
}

//nms:0---2^32/fac_us/1000ms=477218.588ms=477s
void delay_ms(u16 nms)
{	 		  	  
	delay_us((u32)(nms*1000));			 //普通方式延时  
}

void SysTick_Handler(void)
{	
	xitong_haomiao++;
	SysTick_Callback();
}
