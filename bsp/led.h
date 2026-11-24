/******************************************************************************
 * led.h - LED驱动程序头文件
 ******************************************************************************/
#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

void LED_Init(void);
void LED_ON(u8 num);
void LED_OFF(u8 num);

#endif /* __LED_H */
