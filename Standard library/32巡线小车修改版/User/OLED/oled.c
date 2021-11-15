/*   更新日志
V1.0   完成了功能函数的编写
V1.1   解决了显示数字最后一位不能为0的问题。（2018.12.24）
V1.2   删除部分繁杂的代码，提高代码的可移植性。
V1.3   优化了汉字取模方式，可以无需修改的复制粘贴PCtoLCD2002生成的字模
V1.4   修复了一些BUG，增加了带参数的字符串的使用方法说明
*/

/* oled.h 文件中包含参数配置，请打开查看 */
#include "oled.h"

#include "oled_bmp.h"
#include "oled_font.h"

//字符串
u8 string[16];

//如果是IIC接口
#if OLED_INTERFACE == 1
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
   GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);		
   GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
   GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
   GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{  
   GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin); 
   GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
   GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i;
	for(i=0;i<8;i++)		
	{
		GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
		if(IIC_Byte & 0x80)
			GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
		else
			GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
		GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
		IIC_Byte<<=1;
	}
	GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
	GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
	GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
}
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	//写一个字节函数
	//dat 要写入的数据或命令
	//CMD 数据/命令标志位 0命令 1数据
	if(cmd==0)
	{
		IIC_Start();
		Write_IIC_Byte(0x78);         
		Write_IIC_Byte(0x00);		
		Write_IIC_Byte(dat); 
		IIC_Stop();
	}
	else
	{
		IIC_Start();
		Write_IIC_Byte(0x78);		
		Write_IIC_Byte(0x40);			
		Write_IIC_Byte(dat);
		IIC_Stop();		
	}
} 
//如果是SPI接口
#else
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	//写一个字节函数
	//dat 要写入的数据或命令
	//CMD 数据/命令标志位 0命令 1数据
	u8 i;
	if(cmd)
		GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);
	else 
		GPIO_ResetBits(DC_GPIO,DC_GPIO_Pin);
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(SCL_GPIO,SCL_GPIO_Pin);
		if(dat&0x80)
			GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
		else
			GPIO_ResetBits(SDA_GPIO,SDA_GPIO_Pin);
		GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
		dat<<=1;
	}
	GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);
} 
#endif

void OLED_Set_Pos(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
} 

void OLED_Display_On(void)
{
	//开启OLED显示
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xaf,OLED_CMD);

}
void OLED_Display_Off(void)
{
	//关闭OLED显示
	OLED_WR_Byte(0x8d,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0xaf,OLED_CMD);	

}
void OLED_Clear(void)
{
	//清屏
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD);
		OLED_WR_Byte(0x02,OLED_CMD);
		OLED_WR_Byte(0x10,OLED_CMD);	
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	}
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size)
{
	//在指定地方显示一个字符，包括部分字符
	//x：0~127
	//y：0~63
	//size：选择字体16*16或8*6
	u8 c=0,i=0;
	c=chr-' ';
	if(x>127)
	{
		x=0;
		y=y+2;
	}
	if(size == 16)
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);			
		} 
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		
	} 
	else
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
	}	
}

u32 oled_pow(u8 m,u8 n)
{
	//m^n函数
	u32 result = 1;
	while(n--)result*=m;
	return result;
}

void OLED_ShowNum(u8 x,u8 y,long num,u8 len,u8 size)
{ 
	//功能:显示数字
	//x,y :起点坐标	
	//num:+-65535;	                        
	//len :数字的位数，负号算一位
	//size:字体大小
	u32 t,temp1,temp2,temp3,flag; 
	if(num<0)
	{
		flag=1;      //负数标志位置1
		num=-num;   //数字变正数
	}
	for(t=0;t<len;t++)
	{
		temp1=num/oled_pow(10,len-t-1);
		temp2=temp1%10;
		if(flag==1)
		{
			temp3 = num/oled_pow(10,len-t-2);
		}
		if(temp1!=0)
		{
			OLED_ShowChar(x,y,'0'+temp2,size);
			
		}
		else  
		{  
			if(t==len-1&&temp2==0)
			{							
			   OLED_ShowChar(x,y,'0',size);
			}
			else
			{
				if(flag==1&&temp3!=0)      //该数是负数
				{
					OLED_ShowChar(x,y,'-',size);  //输出负号
					flag=0;                       //负号已输出，标志位置0
				}
				else
				{
					OLED_ShowChar(x,y,' ',size);
				}	
			}
		}		
		x+=8;
	}

}

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size)
{
	//显示一个汉字串和字符号串
	//输入\r可以换行
	//写入新汉字需要进行登记
	u8 x0=x;
	u16 k;
	while(*chr!='\0')
	{
		if((*chr)<128)
		{
			if(*chr==13)
			{
				x=x0;
				y+=size/8;
			}
			else
			{
				OLED_ShowChar(x,y,*chr,size);
				x+=8;
				if(x>120)
				{
					x=0;
					y+=size/8;
				}			
			} 
			chr++;
		}
		else if(size==16)        //字体是否为16
		{
			for(k=0;k<hz16_num;k++)  //汉字搜索
			{
				if ((hz16[k].Index[0]==*(chr))&&(hz16[k].Index[1]==*(chr+1)))   //是否是已登记汉字
			   { 
					u8 t;
					OLED_Set_Pos(x,y);
					for(t=0;t<16;t++)
					{
						OLED_WR_Byte(hz16[k].Msk[t],OLED_DATA);
						
					}
					OLED_Set_Pos(x,y+1);
					for(t=0;t<16;t++)
					{
						OLED_WR_Byte(hz16[k].Msk[t+16],OLED_DATA);
					}
				
				}
				
			} 
			if(x>112)
			{
				x=0;
				y+=2;
			}				
			chr+=2;x+=16;
			
		}
		else       //输入错误，忽略汉字
		{
		   chr+=2;
		}
	}
}
void OLED_Draw12864BMP(u8 BMP[])
{
	//显示显示BMP图片128×64
	u16 j=0;
	u8 x,y;
	for(y=0;y<8;y++)
	{
		OLED_Set_Pos(0,y);
		for(x=0;x<128;x++)
		{
			#if DISPLAY_MODE
			OLED_WR_Byte(~BMP[j++],OLED_DATA);
			#else
			OLED_WR_Byte(BMP[j++],OLED_DATA);
			#endif
		}
	}
}
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	//显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
	u16 j=0;
	u8 x,y;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{
			OLED_WR_Byte(BMP[j++],OLED_DATA);
		}
	}
}

void OLED_Init(void)
{
	//初始化需要用到的IO,从头文件修改即可
	//执行初始化OLED代码
	GPIO_InitTypeDef GPIO_InitStructure;
	//SCL  D0
	RCC_APB2PeriphClockCmd(SCL_GPIOClock,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(SCL_GPIO,SCL_GPIO_Pin);
	//SDA  D1
	RCC_APB2PeriphClockCmd(SDA_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(SDA_GPIO,SDA_GPIO_Pin);
	
//如果是SPI接口
#if   OLED_INTERFACE == 2	      
	//RST
	RCC_APB2PeriphClockCmd(RST_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RST_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RST_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);
  //DC
	RCC_APB2PeriphClockCmd(DC_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DC_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DC_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(DC_GPIO,DC_GPIO_Pin);	
	//CS
	RCC_APB2PeriphClockCmd(CS_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CS_GPIO,&GPIO_InitStructure);	
	GPIO_ResetBits(CS_GPIO,CS_GPIO_Pin);	
#endif

//如果需要使用IO口供电
#if OLED_IO_to_POWER == 1
  //VCC
	RCC_APB2PeriphClockCmd(VCC_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = VCC_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(VCC_GPIO,&GPIO_InitStructure);	
	GPIO_SetBits(VCC_GPIO,VCC_GPIO_Pin);	
	//GND
	RCC_APB2PeriphClockCmd(GND_GPIOClock,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GND_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GND_GPIO,&GPIO_InitStructure);	
	GPIO_ResetBits(GND_GPIO,GND_GPIO_Pin);
#endif

//如果是SPI接口
#if   OLED_INTERFACE == 2	
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);
	GPIO_ResetBits(RST_GPIO,RST_GPIO_Pin);
	GPIO_SetBits(RST_GPIO,RST_GPIO_Pin);	
#endif

						  
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00，列地址模式;01，
															//行地址模式;10,页地址模式;默认 10;
	OLED_WR_Byte(0x10,OLED_CMD);//设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//设置显示开始行 [5:0],行数
	OLED_WR_Byte(0x81,OLED_CMD);//对比度设置
	OLED_WR_Byte(0xCF,OLED_CMD);//设定SEG输出电流亮度
	OLED_WR_Byte(0xA1,OLED_CMD);//段重定义设置,bit0:0,0->0;1,0->127;     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//设置 COM 扫描方向;bit3:0,普通模式;1,
															//重定义模式 COM[N-1]->COM0;N:驱动路数   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA8,OLED_CMD);//设置驱动路数
	OLED_WR_Byte(0x3f,OLED_CMD);//默认 0X3F(1/64)
	OLED_WR_Byte(0x81,OLED_CMD);//对比度设置
	OLED_WR_Byte(0xfF,OLED_CMD);//1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD3,OLED_CMD);////设置显示偏移(0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//设置时钟分频因子,震荡频率
	OLED_WR_Byte(0x80,OLED_CMD);//[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);//将预充电作为15个时钟和放电作为1个时钟
	OLED_WR_Byte(0xDA,OLED_CMD);//设置 COM 硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);//[5:4]配置
	OLED_WR_Byte(0xDB,OLED_CMD);//设置 VCOMH 电压倍率
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//设置内存地址模式 (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00，列地址模式;01，
															//行地址模式;10,页地址模式;默认 10;
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵启用/禁用
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	#if DISPLAY_MODE == 1
	OLED_WR_Byte(0xA7,OLED_CMD);//设置显示方式;bit0:1,反相显示;0,正常显示 
	#else
	OLED_WR_Byte(0xA6,OLED_CMD);//设置显示方式;bit0:1,反相显示;0,正常显示	
	#endif
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 	
	OLED_Clear();
	OLED_Set_Pos(0,0);	
}






