#include "oled.h"
#include "font.h"
static void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(OLED_RES_CLK|OLED_DC_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = OLED_RES_Pin|OLED_DC_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OLED_RES_PORT,&GPIO_InitStructure);	
	Software_SPI_Init();
	/*复位寄存器*/
	OLED_RES_0();
	SysTick_Delay_Ms(200);
	OLED_RES_1();
}

static void OLED_WriteCmd(u8 Cmd)
{
	SPI_CS_0();
	OLED_DC_Cmd();//DC引脚低电平表示命令
	Software_SPI_Write(Cmd);
	SPI_CS_1();
}

static void OLED_WriteData(u8 Cmd)
{
	SPI_CS_0();
	OLED_DC_Data();//DC引脚高电平表示数据
	Software_SPI_Write(Cmd);
	SPI_CS_1();
}

void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WriteCmd(0XB0+i);//设置页地址 0XB0~0XB7
		OLED_WriteCmd(0X00);//设置显示位置—列低地址
		OLED_WriteCmd(0X10);//设置显示位置—列高地址
		for(n=0;n<128;n++)
		{
			OLED_WriteData(0);
		}
	}
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x00); 
}   

void OLED_Init(void)
{
	OLED_GPIO_Init();
	OLED_WriteCmd(0XAE);//关闭OLED面板
	OLED_WriteCmd(0X00);//设置页地址模式的列起始地址低位
	OLED_WriteCmd(0X10);//设置页地址模式的列起始地址高位
	OLED_WriteCmd(0X40);//设置屏幕起始行
	
	OLED_WriteCmd(0X81);//设置对比度(相当于屏幕亮度)
	OLED_WriteCmd(0X00);//对比度设置值，范围0X00-0XFF
	
	/*好像是设置显示方向的*/
	OLED_WriteCmd(0XA1);//设置行扫描方向为从左到右
	OLED_WriteCmd(0XC8);//设置列扫描方向为数据低位在前
	
	
	OLED_WriteCmd(0XA8);//设置复用率
	
	OLED_WriteCmd(0X3F);
	
	/*显示偏移*/
	OLED_WriteCmd(0XD3);//设置显示偏移
	OLED_WriteCmd(0x00);//不偏移
	
	/*设置时钟*/
	OLED_WriteCmd(0XD5);//设置显示时钟分频值/震荡频率
	OLED_WriteCmd(0x80);//设置分频比，将时钟设置为100帧/秒
	
	/*充电*/
	OLED_WriteCmd(0XD9);//设置预充电周期
	OLED_WriteCmd(0xF1);//1个充电时钟和15个放电时钟
	
	OLED_WriteCmd(0XDA);//设置列引脚硬件配置
	OLED_WriteCmd(0x12);//列输出扫描方向从COM63到COM0(C8h), 启用列左/右映射(DAh A[5]=1)

	OLED_WriteCmd(0XDB);//设置VCOMH反压值
	OLED_WriteCmd(0X40);//设置VCOM取消选择级别
	
	OLED_WriteCmd(0X20);//设置内存寻址模式
	OLED_WriteCmd(0x02);//页地址寻址模式
	
	OLED_WriteCmd(0x8D);//电荷泵启用/禁用
	OLED_WriteCmd(0x14);//关闭
	
	OLED_WriteCmd(0xA4);//点亮屏幕
	OLED_WriteCmd(0xA6);//设置正常显示，不反转，1表示点亮像素
	OLED_WriteCmd(0xAF);//打开OLED面板
	
	OLED_Clear();//清屏
	OLED_Set_Pos(0,0);//设置起始坐标
}

/*
 * @brief 显示一个汉字
 * @para  x 横坐标 
 * @para  y 纵坐标
*/
void OLED_ShowChar(u8 x,u8 y,u8 Char)
{
	u8 t;
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++)
	{
		OLED_WriteData(chinese[2*Char][t]);
  }
	OLED_Set_Pos(x,y+1);
  for(t=0;t<16;t++)
	{	
			OLED_WriteData(chinese[2*Char+1][t]);
  }			
}

/*
 * @brief 显示多个汉字
 * @para  x 横坐标   
 * @para  y 纵坐标
 * @para  length 待显示汉字的长度
*/
void OLED_ShowCHinese(u8 x,u8 y,u8 length)
{
	u8 i;
	for(i=0;i<length;i++)
	{
		OLED_ShowChar(x,y,i);
		x += 16;
	}
}

/*
 * @brief 显示8x16字符串
 * @para  x  横坐标
 * @para  y  纵坐标
 * @para  ch 指针
*/
void OLED_8x16Str(u8 x,u8 y,char *ch)
{
	u8 c=0,i=0,j=0;
	while(ch[j]!='\0')
	{
		c = ch[j]-32;
		if(x>120)//换行显示
		{
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WriteData(F8X16[c*16+i]);//前8位字模数据
		}
		OLED_Set_Pos(x,y+1);//下一页
		for(i=0;i<8;i++)
		{
			OLED_WriteData(F8X16[c*16+i+8]);//前8位字模数据
		}
		x+=8;//下一个字符的坐标
		j++;//字符的序列号
	}
}

