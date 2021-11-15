#include "delay.h"
                                   // ms    s    h  day year
volatile u64 xitong_haomiao;       //2^64/1000/3600/24/365=584942417��Ḵλ
//USART1_printf("xitong_haomiao=%llu\r\n",xitong_haomiao);
volatile u64 xitong_haomiao_old=0; 	   
	   
static   u8  fac_us=0;					   //us��ʱ������	
 
void delay_init()
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);    //1���򿪸���ʱ�ӣ�
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//2���ر�jtag ����sw
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	 //ѡ���ⲿʱ��  HCLK/8  9M ��������1Ϊ1/9000000��
	fac_us=SystemCoreClock/8000000;				                 //9  
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	           //����SYSTICK�ж�
	SysTick->LOAD=fac_us*1000-1; 						               //ÿ1/1000s�ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	           //����SYSTICK    
}	

//nus:0~2^32/fac_us=477218588.444us=477s   								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*fac_us; 							  //��Ҫ�Ľ�����	  		 
	tcnt=0;
	told=SysTick->VAL;        			  //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				  //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}	
}

//nms:0---2^32/fac_us/1000ms=477218.588ms=477s
void delay_ms(u16 nms)
{	 		  	  
	delay_us((u32)(nms*1000));			 //��ͨ��ʽ��ʱ  
}

void SysTick_Handler(void)
{	
	xitong_haomiao++;
	SysTick_Callback();
}
