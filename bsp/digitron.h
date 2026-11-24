/******************************************************************************
 * digitron.h - 数码管驱动头文件 (74HC595)
 * 硬件连接:
 *   DS (数据)    → PD5
 *   SH_CP (移位) → PD7
 *   ST_CP (锁存) → PD6
 *   OE (使能)    → PD12 (低电平有效)
 *   两个级联74HC595: U7=位选, U8=段选
 *   8位共阴极数码管
 ******************************************************************************/
#ifndef __DIGITRON_H
#define __DIGITRON_H

#include "stm32f4xx.h"

void Digitron_Init(void);
void Digitron_Display(uint8_t pos, uint8_t num);   // num: 0~15 (十六进制 0~F)

#endif /* __DIGITRON_H */
