#ifndef _DELAY_H_
#define _DELAY_H_ 	
#include "sys.h"  

#include "SysTick.h"

extern volatile u64 xitong_haomiao;
extern volatile u64 xitong_haomiao_old; 

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























