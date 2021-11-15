#ifndef __OLEDFONT_H
#define __OLEDFONT_H 	 

#define hz16_num   54  //汉字个数

struct typFNT_GB162    //汉字用的结构体
{
	 char Msk[32];
	 unsigned char Index[2];	
	 unsigned char num;  
};
extern const struct typFNT_GB162 hz16[];
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];


#endif

