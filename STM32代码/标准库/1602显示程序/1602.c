#include <reg52.h>

sbit LCD_RS = P3^5;
sbit LCD_RW = P3^6;
sbit LCD_EN = P3^4;

#define LCD_Data  P0
#define uchar unsigned char





//5ms��ʱ
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}


//�ж�æ
void JudgeBusy()
{
	//51�ⲿ�����ʱ�����ѵ�ƽ���ߣ���ΪIO��Ϊ0���޷������ߡ�
	LCD_Data = 0XFF;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 0;
	LCD_EN = 1;
	//LCD_EN = 0;
	while(LCD_Data&0X80);
}

//д����
void WriteOrder(uchar Order)
{
	JudgeBusy();//���æ
	LCD_Data = Order;
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	LCD_EN = 1;
//	LCD_EN = 0;
}

//д����
void WriteData(uchar Data)
{
	JudgeBusy();//���æ
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
	WriteOrder(0X0C);//����� ָ��4
	WriteOrder(0X01);//����ʾ AC=0 �Զ���1 ָ��1
}

void SendData(uchar X,uchar Y,uchar Data)
{
	Y &= 0X01;//Y���ܴ���1
	X &= 0XFF;//X���ܴ���15;
	if(Y)
		X |= 0X40;//�ڶ��ŵ�ַ+OX40
	X |= 0X80;//DDRAM��ַDB7Ĭ��Ϊ1,7Ϊ��ַ
	WriteOrder(X);
	WriteData(Data);
}


//��ָ��λ����ʾһ���ַ�
void DisplayListChar(uchar X, uchar Y, uchar code *DData)
{
 unsigned char ListLength;

  ListLength = 0;
 Y &= 0x1;
 X &= 0xF; //����X���ܴ���15��Y���ܴ���1
 while (DData[ListLength]>=0x20) //�������ִ�β���˳� ASCII 0X20-�ո�
  {
   if (X <= 0xF) //X����ӦС��0xF
    {
     SendData(X, Y, DData[ListLength]); //��ʾ�����ַ�
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
//	SendData(0,1,0XFC);//��CGRAM�ж�ȡ����
	while(1);
}