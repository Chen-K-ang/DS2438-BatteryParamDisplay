#ifndef LED_BUZZ_H
#define LED_BUZZ_H

#include "sys.h"

#define LED  PBout(7)   //LED接口
#define BUZZ PBout(14)	//蜂鸣器接口

void led_buzz_init(void);

#endif
