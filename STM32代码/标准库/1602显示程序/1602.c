#include <reg52.h>

sbit LCD_RS = P3^5;
sbit LCD_RW = P3^6;
sbit LCD_EN = P3^4;

#define LCD_Data  P0
#define uchar unsigned char





//5ms延时
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}


//判断忙
void JudgeBusy()
{
	//51外部输入的时候必须把电平拉高，因为IO口为0的无法被拉高。
	LCD_Data = 0XFF;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 0;
	LCD_EN = 1;
	//LCD_EN = 0;
	while(LCD_Data&0X80);
}

//写命令
void WriteOrder(uchar Order)
{
	JudgeBusy();//检测忙
	LCD_Data = Order;
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_EN = 1;
//	LCD_EN = 0;
}

//写数据
void WriteData(uchar Data)
{
	JudgeBusy();//检测忙
	LCD_Data = Data;
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_EN = 1;
	//LCD_EN = 0;
}

void _1602_Init()
{
	WriteOrder(0X38);
	Delay5Ms();
	WriteOrder(0X0C);//开光标 指令4
	WriteOrder(0X01);//清显示 AC=0 自动加1 指令1
}

void SendData(uchar X,uchar Y,uchar Data)
{
	Y &= 0X01;//Y不能大于1
	X &= 0XFF;//X不能大于15;
	if(Y)
		X |= 0X40;//第二排地址+OX40
	X |= 0X80;//DDRAM地址DB7默认为1,7为地址
	WriteOrder(X);
	WriteData(Data);
}


//按指定位置显示一串字符
void DisplayListChar(uchar X, uchar Y, uchar code *DData)
{
 unsigned char ListLength;

  ListLength = 0;
 Y &= 0x1;
 X &= 0xF; //限制X不能大于15，Y不能大于1
 while (DData[ListLength]>=0x20) //若到达字串尾则退出 ASCII 0X20-空格
  {
   if (X <= 0xF) //X坐标应小于0xF
    {
     SendData(X, Y, DData[ListLength]); //显示单个字符
     ListLength++;
     X++;
    }
  }
}

void main()
{
	uchar code a[] = {"Hello Wold"};
	_1602_Init();
	DisplayListChar(0,0,&a);
//	SendData(0,1,0XFC);//从CGRAM中读取数据
	while(1);
}