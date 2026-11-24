/******************************************************************************
 * switch.h - 开关驱动程序头文件
 * 硬件连接: 开关接在GPIOF Pin0~Pin7，按下为高电平，松开为低电平
 ******************************************************************************/
#ifndef __SWITCH_H
#define __SWITCH_H

#include "stm32f4xx.h"

void Switch_Init(void);
uint8_t Switch_Read(uint8_t pin);
void Switch_EXTI_Init(void);

#endif /* __SWITCH_H */
