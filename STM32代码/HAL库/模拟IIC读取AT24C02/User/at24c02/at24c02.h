#ifndef _AT24C02_H
#define _AT24C02_H

#include "stm32f1xx.h"
#include "main.h"
void AT24C02_ByteWrite(uint8_t addr,uint8_t data);
uint8_t CurAddr_Read(void);
void Page_Write(uint8_t addr,uint8_t * data,uint8_t size);
void Seq_Read(uint8_t addr,uint8_t *data,uint8_t size);
#endif
