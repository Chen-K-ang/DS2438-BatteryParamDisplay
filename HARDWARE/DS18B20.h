#ifndef DS18B20_H
#define DS18B20_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "ds18b20.h"
#include "usart.h"
 
/*????*/
 
//???DS18B20?????
#define DS18B20_INPUT_MODE() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x80000000;}
 
//???DS18B20?????
#define DS18B20_OUTPUT_MODE(){GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x30000000;}
 
//DS18B20 IO???
#define DS18B20_OUT PBout(15)
 
//DS18B20 IO???
#define DS18B20_IN PBin(15) 
 
//????
u8 DS18B20_CleckAck(void);
u8 DS18B20_CheckDevice(void);
void DS18B20_Init(void);
u16 DS18B20_ReadTemp(void);
u8 DS18B20_ReadByte(void);
void DS18B20_WriteByte(u8 cmd);
#endif
