#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int

sbit DQ=P3^7;
sbit light=P2^0;
sbit k1=P3^4;
sbit k2=P3^3;
sbit k3=P3^2;
sbit k4=P3^5;
sbit k5=P3^6;
sbit LED1=P1^3;
sbit LED2=P1^4;

sbit DS1302_CE=P1^2;
sbit DS1302_IO=P1^1;
sbit DS1302_SCLK=P1^0;

sbit LCD1602_RS=P2^5;
sbit LCD1602_RW=P2^6;
sbit LCD1602_EN=P2^7;

extern uchar wendu[4];
extern void get_wendu();

extern void LCD1602_init();
extern void LCD1602_wcmd(uchar cmd);
extern void LCD1602_wdat(uchar dat);
extern void LCD1602_pos(uchar x,bit y);         //定义显示位置，x为列，y为行，0上1下
extern void LCD1602_dischar(uchar x,bit y,uchar dat); //在指定位置显示字符
extern void LCD1602_prints(uchar x,bit y,uchar *p);  //显示字符串

extern uchar dt[7];
extern void set_time();
extern void get_time();
extern void DS1302_init();

/**************************DS1302 *********************************/

uchar dt[7];

void DS1302_write_byte(uchar dat)
{
  char i;

  for(i=0;i<8;i++)
  {
    DS1302_SCLK = 0;
        _nop_();
        _nop_();
        dat >>= 1;
        DS1302_IO = CY;
        DS1302_SCLK=1;
        _nop_();
        _nop_();
  }
}

uchar DS1302_read_byte()
{
  uchar i;
  uchar dat = 0;
  for(i=0;i<8;i++)
  {
    DS1302_SCLK = 0;
        _nop_();
        _nop_();
        dat >>= 1;
        if(DS1302_IO) dat |= 0x80;
        DS1302_SCLK =1;
        _nop_();
        _nop_();
  }
  return dat;
}


void DS1302_write(uchar address,uchar dat)
{
  DS1302_CE=0;
  _nop_();
  _nop_();
  DS1302_SCLK=0;
  _nop_();
  _nop_();
  DS1302_CE=1;
  _nop_();
  _nop_();
  DS1302_write_byte(address);
  DS1302_write_byte(dat);
  DS1302_SCLK=1;
  DS1302_CE=0;
}


uchar DS1302_read(uchar address)
{
  uchar dat;
  DS1302_CE=0;
  _nop_();
  _nop_();                                         
  DS1302_SCLK=0;
  _nop_();
  _nop_();
  DS1302_CE=1;
  _nop_();
  _nop_();
  DS1302_write_byte(address);
  dat =DS1302_read_byte();
  DS1302_SCLK=1;
  DS1302_CE=0;
  dat=dat/16*10+dat%16;
  return dat;
}

void DS1302_init()
{
  DS1302_write(0x8e,0x00);
  DS1302_write(0x80,0x00);
  DS1302_write(0x82,0x00);
  DS1302_write(0x84,0x12);
  DS1302_write(0x86,0x10);
  DS1302_write(0x88,0x11);
  DS1302_write(0x8a,0x06);
  DS1302_write(0x8c,0x12);
  DS1302_write(0x8e,0x80);
}

void set_time()
{
  uchar i,j=0x80;
  for(i=0;i<7;i++)
  {
    dt[i]=dt[i]/10*16+dt[i]%10;
  }
  DS1302_write(0x8e,0x00);
  DS1302_write(0x80,0x00);
  for(i=0;i<7;i++)
  {
    DS1302_write(j,dt[i]);
    j+=2;
  }
  DS1302_write(0x8e,0x80);
}

void get_time()
{
  dt[0] =DS1302_read(0x81);		 //读1302时间，秒
  dt[1] =DS1302_read(0x83);		 //分
  dt[2] =DS1302_read(0x85);		 //时
  dt[3] =DS1302_read(0x87);		 //日
  dt[4] =DS1302_read(0x89);		 //月
  dt[5] =DS1302_read(0x8b);		 //？周
  dt[6] =DS1302_read(0x8d);		 //年
}

void chaongdiaon()                                   //充电
{
DS1302_write(0x8e,0x00);           /* 控制命令,WP=0,解除写保护*/
  _nop_();
  _nop_();
DS1302_write(0x90, 0xa6);    //使能充电   用一个二极管  用4k电阻
DS1302_write(0x8e,0x80);           /* 控制命令,WP=1,写保护*/
  _nop_();
  _nop_();
}
/*********************************   LCD  1602      ******************/
uchar code self_char[]={
                                                0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02, //年
                                                0x0f,0x09,0x0f,0x09,0x0f,0x09,0x13,0x01, //月
                                                0x0f,0x09,0x09,0x0f,0x09,0x09,0x0f,0x00, //日
                                                0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00, //温度标志— —摄氏度
                                                0x00,0x04,0x0E,0x1F,0x0E,0x04,0x00,0x00, //符号◆
                                                0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, //全开
                                                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //
                                                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00         //
                                           };

bit lcd_busy()
{
  bit result;
  LCD1602_RS=0;
  LCD1602_RW=1;
  LCD1602_EN=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (bit)(P0&0x80);
  LCD1602_EN=0;
  return(result);
}

void LCD1602_wcmd(uchar cmd)
{
  while(lcd_busy());
  LCD1602_RS=0;
  LCD1602_RW=0;
  _nop_();
  _nop_();
  LCD1602_EN=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  P0=cmd;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD1602_EN=0;
}

void LCD1602_wdat(uchar dat)
{
  while(lcd_busy());
  LCD1602_RS=1;
  LCD1602_RW=0;
  LCD1602_EN=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  P0=dat;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  LCD1602_EN=0;
}

void LCD1602_pos(uchar x,bit y)                //指定坐标,x为列，0~15，y为行，0为第一行，1为第二行。
{
  if(y)LCD1602_wcmd(x|0xc0);
  else LCD1602_wcmd(x|0x80);
}

void LCD1602_dischar(uchar x,bit y,uchar dat)        //指定位置显示一个字符
{
  LCD1602_pos(x,y);
  LCD1602_wdat(dat);
}

void LCD1602_prints(uchar x,bit y,uchar *p)                                        //指定位置显示字符串
{
  LCD1602_pos(x,y);
  while((*p) != '\0') {LCD1602_wdat(*p);p++;}
}

void LCD1602_init()
{
  uchar i;
  LCD1602_wcmd(0x40);
  for(i=0;i<64;i++)
        {
          LCD1602_wdat(self_char[i]);
        }
  LCD1602_wcmd(0x38);
  LCD1602_wcmd(0x38);
  LCD1602_wcmd(0x0c);
  LCD1602_wcmd(0x06);
  LCD1602_wcmd(0x01);
}
/****************************   18B20   ************************************/
uchar wendu[4];

void delay_18b20(uchar i)
{
  int j;
  for(j=0;j<i;j++);
}

uchar init_18b20()
{
  uchar i;
  DQ=0;
  delay_18b20(30);
  DQ=1;
  delay_18b20(3);
  i=DQ;
  delay_18b20(20);
  return(i);
}

uchar read_18b20_bit()
{
  uchar i;
  DQ=0;
  DQ=1;
  for(i=0;i<3;i++);
  return(DQ);
}

void write_18b20_bit(unsigned xbit)
{
  DQ=0;
  if(xbit==1)
  DQ=1;
  delay_18b20(5);
  DQ=1;
}

uchar read_18b20_byte()
{
  uchar i,j,value;
  j=1;
  value=0;
  for(i=0;i<8;i++)
  {
        if(read_18b20_bit())
        {
          value=value+(j<<i);
        }
        delay_18b20(6);
  }
  return(value);
}

void write_18b20_byte(uchar x)
{
  uchar i,j;
  for(i=0;i<8;i++)
  {
        j=x>>i;
        j=j&0x01;
        write_18b20_bit(j);
        delay_18b20(5);
  }
}

void get_wendu()
{
  uchar a,b;
  float get_temp;
  int temp1,temp2;
  init_18b20();
  write_18b20_byte(0xcc);
  write_18b20_byte(0x44);
  init_18b20();
  write_18b20_byte(0xcc);
  write_18b20_byte(0xbe);
  a=read_18b20_byte();         //低8位
  b=read_18b20_byte();         //高8位
  temp1=b;
  temp1=temp1<<8;
  temp1=temp1|a;
  if(b&0xfc)
        {wendu[0]=1;temp1=~temp1+1;}                //0正1负
  else 
    wendu[0]=0;
  get_temp=temp1*0.0625;
  temp2=get_temp*10+0.5;
  wendu[1]=temp2%1000/100;
  wendu[2]=temp2%1000%100/10;
  wendu[3]=temp2%1000%100%10;
}


/*******************************          主程序         ***************/

 uchar mode=0,OK=0;
uchar dis1[16],dis2[16];
//uchar IRCOM[7],IRCODE=0;
uchar NUM1,NUM2;
uchar count;
bit L=0,select=0,DIS_EN=1,SHAN;

void delay(uint i)
{
  uchar j;
  while(i-- !=0)
  {
    for(j=0;j<120;j++);
  }
}

void delay1(uchar i)
{
  uchar j;
  while(i--)
  {for(j=0;j<13;j++){}}
}


void get_dis()
{
  dis1[0]='2';
  dis1[1]='0';
  dis1[2]=0x30+dt[6]/10;
  dis1[3]=0x30+dt[6]%10;
  dis1[4]='-';		//2
  dis1[5]=0x30+dt[4]/10;
  dis1[6]=0x30+dt[4]%10;
  dis1[7]='-';		//1
  dis1[8]=0x30+dt[3]/10;
  dis1[9]=0x30+dt[3]%10;
  dis1[10]=' ';	    //2
  dis1[11]=' ';
  dis1[12]=' ';
  switch(dt[5])
  {
        case 1: {
                          dis1[13]='M';
                          dis1[14]='o';
                          dis1[15]='n';
                        }
                        break;
        case 2: {
                          dis1[13]='T';
                          dis1[14]='u';
                          dis1[15]='e';
                        }
                        break;
        case 3: {
                          dis1[13]='W';
                          dis1[14]='e';
                          dis1[15]='d';
                        }
                        break;
        case 4: {
                          dis1[13]='T';
                          dis1[14]='h';
                          dis1[15]='u';
                        }
                        break;
        case 5: {
                          dis1[13]='F';
                          dis1[14]='r';
                          dis1[15]='i';
                        }
                        break;
        case 6: {
                          dis1[13]='S';
                          dis1[14]='a';
                          dis1[15]='t';
                        }
                        break;
        case 7: {
                          dis1[13]='S';
                          dis1[14]='u';
                          dis1[15]='n';
                        }
                        break;
  }
  dis2[0]=' ';
  dis2[1]=0x30+dt[2]/10;
  dis2[2]=0x30+dt[2]%10;
  dis2[3]=':';			   //;:
  dis2[4]=0x30+dt[1]/10;
  dis2[5]=0x30+dt[1]%10;
  dis2[6]=':';
  dis2[7]=0x30+dt[0]/10;
  dis2[8]=0x30+dt[0]%10;
  dis2[9]=' ';
  if(wendu[0]==1)
        dis2[10]=0x2d;
  else
        dis2[10]=' ';
  dis2[11]=0x30+wendu[1];
  dis2[12]=0x30+wendu[2];
  dis2[13]='.';
  dis2[14]=0x30+wendu[3];
  dis2[15]=3;
}

void keyscan()
{
  if(k1==0)
  {
        delay(10);
        if(k1==0)
        {
           while(!k1);

          mode++;
          if(mode==1 && OK==0)
                LCD1602_wcmd(0x01);         
          if(mode==2 && OK==0)        
          {
                mode=0;
                select=0;
          }
          if(select==0 && OK==1)
          {
                if(mode==7)
                  mode=1;
          }
          if(select==1 && OK==1)
          {
                if(mode==6)
                  mode=1;
          }

        }
  }
 if(k2==0)
  {
        delay(10);
        if(k2==0)
        {
          while(!k2);
          if(mode==1 && OK==0)
            {select=~select;LCD1602_wcmd(0x01);}
          NUM1++;

        }
  }
  if(k3==0)
  {
        delay(10);
        if(k3==0)
        {
          while(!k3);
          if(mode==1 && OK==0)
            {select=~select;LCD1602_wcmd(0x01);}
          NUM2++;

        }
  }
  if(k4==0)
  {
        delay(10);
        if(k4==0)
        {
          while(!k4);
          if(mode==0)
                light=~light;
          if(mode !=0)
          {
            LCD1602_wcmd(0x01);
                OK++;
                NUM1=0;
                NUM2=0;
                if(OK==2)
                {
                  LCD1602_wcmd(0x01);

                  set_time();
//                  set_alarm();
                  mode=0;
                  select=0;
                  OK=0;
                }
          }

        }
  }
  if(k5==0)
  {
        delay(100);
        if(k5==0)
        {
          while(!k5);
          DS1302_init();

        }
  }        
}        





void display()
{
  get_wendu();
  get_dis();
/*  if(alarm[0]==1)
        {LED2=1;}  */

  if(mode==0)        //正常模式                                         
  {
        uchar i;
        LED1=1;
        get_time();
//        get_alarm();
        LCD1602_pos(0,0);
        for(i=0;i<16;i++)
          LCD1602_wdat(dis1[i]);
        LCD1602_pos(0,1);
        for(i=0;i<16;i++)
          LCD1602_wdat(dis2[i]);
  }

  if(mode==1&&OK==0)  //菜单模式
  {
        LCD1602_prints(1,0,"1.Set Time");
        LCD1602_prints(1,1,"2.About...");
        if(select)
          LCD1602_dischar(0,1,4);
        else
          LCD1602_dischar(0,0,4);
  }

  if(OK==1&&select==0)          //调日期模式
  {
        uchar i;
        LCD1602_pos(0,0);
        for(i=0;i<16;i++)
          LCD1602_wdat(dis1[i]);
        LCD1602_pos(0,1);
        for(i=0;i<10;i++)
          LCD1602_wdat(dis2[i]);
        LCD1602_prints(10,1,"Set...");
        if(SHAN==1) LCD1602_prints(10,1,"Set.  ");

        if(mode==1)                  //调分模式，秒位清零
        {
          dt[0]=0;
          LCD1602_dischar(7,1,0x30+dt[0]/10);
          LCD1602_dischar(8,1,0x30+dt[0]%10);
          if(NUM1!=0)
                {
                  dt[1]++;
                  if(dt[1]==60)
                        dt[1]=0;
                  LCD1602_dischar(4,1,0x30+dt[1]/10);
                  LCD1602_dischar(5,1,0x30+dt[1]%10);
                  NUM1=0;
                }
                if(NUM2!=0)
                {
                  dt[1]--;
                  if(dt[1]==-1)
                        dt[1]=59;
                  LCD1602_dischar(4,1,0x30+dt[1]/10);
                  LCD1602_dischar(5,1,0x30+dt[1]%10);
                  NUM2=0;
                }
                if(SHAN==1) LCD1602_dischar(5,1,5);
                
        }
          else

           if(mode==2)         //调时模式
           {
                    if(NUM1!=0)
                 {
                   dt[2]++;
                   if(dt[2]==24)
                         dt[2]=0;
                   LCD1602_dischar(1,1,0x30+dt[2]/10);
                   LCD1602_dischar(2,1,0x30+dt[2]%10);
                   NUM1=0;
                 }
                 if(NUM2!=0)
                 {
                   dt[2]--;
                   if(dt[2]==-1)
                         dt[2]=23;
                   LCD1602_dischar(1,1,0x30+dt[2]/10);
                   LCD1602_dischar(2,1,0x30+dt[2]%10);
                   NUM2=0;
                 }
                  if(SHAN==1) LCD1602_dischar(2,1,5);
            }
                if(mode==3)        //调星期状态
           {
                    if(NUM1!=0)
                 {
                   dt[5]++;
                   if(dt[5]==8)
                         dt[5]=1;
                   switch(dt[5])
                   {
                            case 1: LCD1602_prints(13,0,"Mon");break;
                         case 2: LCD1602_prints(13,0,"Tue");break;
                         case 3: LCD1602_prints(13,0,"Wed");break;
                         case 4: LCD1602_prints(13,0,"Thu");break;
                         case 5: LCD1602_prints(13,0,"Fri");break;
                         case 6: LCD1602_prints(13,0,"Sat");break;
                         case 7: LCD1602_prints(13,0,"Sun");break;
                   }
                   NUM1=0;
                 }
                 if(NUM2!=0)
                 {
                   dt[5]--;
                   if(dt[5]==0)
                         dt[5]=7;
                   switch(dt[5])
                   {
                            case 1: LCD1602_prints(13,0,"Mon");break;
                         case 2: LCD1602_prints(13,0,"Tue");break;
                         case 3: LCD1602_prints(13,0,"Wed");break;
                         case 4: LCD1602_prints(13,0,"Thu");break;
                         case 5: LCD1602_prints(13,0,"Fri");break;
                         case 6: LCD1602_prints(13,0,"Sat");break;
                         case 7: LCD1602_prints(13,0,"Sun");break;
                   }
                   NUM2=0;
                 }
                  if(SHAN==1) LCD1602_dischar(15,0,5);
            }
                if(mode==4)          //调日模式
           {
                    if(NUM1!=0)
                 {
                   dt[3]++;
                   if(dt[3]==32)
                         dt[3]=1;
                   LCD1602_dischar(8,0,0x30+dt[3]/10);
                   LCD1602_dischar(9,0,0x30+dt[3]%10);
                   NUM1=0;
                 }
                 if(NUM2!=0)
                 {
                   dt[3]--;
                   if(dt[3]==0)
                         dt[3]=31;
                   LCD1602_dischar(8,0,0x30+dt[3]/10);
                   LCD1602_dischar(9,0,0x30+dt[3]%10);
                   NUM2=0;
                 }
                  if(SHAN==1) LCD1602_dischar(9,0,5);
            }
                if(mode==5)                //调月模式
           {
                    if(NUM1!=0)
                 {
                   dt[4]++;
                   if(dt[4]==13)
                         dt[4]=1;
                   LCD1602_dischar(5,0,0x30+dt[4]/10);
                   LCD1602_dischar(6,0,0x30+dt[4]%10);
                   NUM1=0;
                 }
                 if(NUM2!=0)
                 {
                   dt[4]--;
                   if(dt[4]==0)
                         dt[4]=12;
                   LCD1602_dischar(5,0,0x30+dt[4]/10);
                   LCD1602_dischar(6,0,0x30+dt[4]%10);
                   NUM2=0;
                 }
                  if(SHAN==1) LCD1602_dischar(6,0,5);
            }
                if(mode==6)          //调年模式
           {
                    if(NUM1!=0)
                 {
                   dt[6]++;
                   if(dt[6]==100)
                         dt[6]=0;
                   LCD1602_dischar(2,0,0x30+dt[6]/10);
                   LCD1602_dischar(3,0,0x30+dt[6]%10);
                   NUM1=0;
                 }
                 if(NUM2!=0)
                 {
                   dt[6]--;
                   if(dt[6]==-1)
                         dt[6]=99;
                   LCD1602_dischar(2,0,0x30+dt[6]/10);
                   LCD1602_dischar(3,0,0x30+dt[6]%10);
                   NUM2=0;
                 }
                  if(SHAN==1) LCD1602_dischar(3,0,5);
            }
  }

  if(OK==1&&select==1)        //调闹铃模式
  {


          LCD1602_prints(0,0," Birthday gift  ");
          LCD1602_prints(0,1,"        --by HJL");

          delay(2000);
          LCD1602_wcmd(0x01);
          LCD1602_prints(0,0,"2013-12-04 Is   ");
          LCD1602_prints(0,1,"LiYunL birthday!");

          delay(2000);
          LCD1602_wcmd(0x01);
          LCD1602_prints(0,0,"Friends together");
          LCD1602_prints(0,1,"for life...     ");

          delay(2000);
          LCD1602_wcmd(0x01);
            mode=1 ;
          OK=0;
  }
}
void diy()
{
  LCD1602_init();
  delay(1);
  LCD1602_prints(0,0," Digital Clock  ");
  LCD1602_prints(0,1,"v1.1.0  --by HJL");
  delay(1000);
  LCD1602_wcmd(0x01);             
}



void main()
{
  IT0=1;          //外部中断0为下降沿触发
  EX0=1;          //
  TMOD=0x01;
  TH0=0x3C;                //50ms
  TL0=0xB0;
  EA=1;
  ET0=1;
  TR0=1;

  get_wendu();

  diy();
  chaongdiaon();



  while(1)
  {
    P1=0xff;
          keyscan();
        display();
  }
}



void timer0() interrupt 1
{
  TH0=0x3C;
  TL0=0xB0;
  count++;
  if(count==10)
  {
        count=0;
        SHAN=~SHAN;
  }
} 
