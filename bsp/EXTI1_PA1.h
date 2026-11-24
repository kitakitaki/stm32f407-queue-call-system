/******************************************************************************
 * EXTI1_PA1.h - EXTI1外部中断驱动 (PA1取号按键)
 * 硬件连接: 按键接PA1，上拉输入，下降沿触发
 * 说明: 初始化后，用户需自行实现 EXTI1_IRQHandler 中断服务函数
 ******************************************************************************/
#ifndef __EXTI1_PA1_H
#define __EXTI1_PA1_H

#include "stm32f4xx.h"

/**
 * @brief  初始化EXTI1 (PA1) 为下降沿触发中断模式
 * @note   本函数包含GPIO、EXTI、NVIC配置
 *         中断服务函数 EXTI1_IRQHandler 需用户自行编写
 */
void EXTI1_PA1_Init(void);

#endif /* __EXTI1_PA1_H */
