/******************************************************************************
 * Timer2.h - TIM2定时器驱动程序头文件
 * 功能: TIM2定时中断初始化及中断服务函数框架
 * 说明: TIME2_IRQHandler中已给出中断标志判断与清除，
 *       if内部的代码由用户自行编写
 ******************************************************************************/
#ifndef __TIMER2_H
#define __TIMER2_H

#include "stm32f4xx.h"

/**
 * @brief  初始化TIM2定时器
 * @param  arr: 自动重装载值
 * @param  psc: 时钟预分频值
 * @note   计数时钟 = 84MHz / (psc+1)
 *         定时周期 = (arr+1) * (psc+1) / 84MHz (单位:秒)
 *         例如: arr=9999, psc=8399 => 定时1秒
 */
void TIM2_Init(u16 arr, u16 psc);


// void TIM2_IRQHandler(void);

#endif /* __TIMER2_H */
