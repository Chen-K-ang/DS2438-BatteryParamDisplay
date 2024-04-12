#ifndef DS2438_H
#define DS2438_H

#include "sys.h"

#define PB12H PBout(12) = 1
#define PB12L PBout(12) = 0

#define  R    0.05

#define V_CC 3000 //µÁ≥ÿ»›¡ø

extern float A, B, C, D, E, F;
extern float I, J;

void ds2438_initcommand(void);
void ds2438_test(void);

float Read_Current(void);
float Read_Temperature(void);
float Read_Voltage(void);
float Read_ICAcurrent(void);
float Read_CCAcurrent(void);
float Read_DCAcurrent(void);

u8 ds2438_reset(void);
void ds2438_wr_byte(u8 cmd);
u8 ds2438_rd_byte(void);

#endif
