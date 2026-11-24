/******************************************************************************
 * EXTI1_PA1.c - EXTI1外部中断驱动 (PA1取号按键)
 * 硬件连接: 按键接PA1，上拉输入，下降沿触发
 * 说明: 初始化后，用户需自行实现 EXTI1_IRQHandler 中断服务函数
 ******************************************************************************/
#include "EXTI1_PA1.h"

/**
 * @brief  初始化EXTI1 (PA1) 为下降沿触发中断模式
 *         PA1上拉输入 → 按下=低电平，松开=高电平
 *         下降沿触发 → 按下瞬间触发中断
 * @note   中断服务函数 EXTI1_IRQHandler 需用户自行编写
 */
void EXTI1_PA1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // PA1 输入模式，上拉（按下=低电平）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 连接EXTI线1到PA1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

    // 下降沿触发
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 清除可能的遗留pending位，防止使能NVIC后立即触发中断 */
    EXTI_ClearITPendingBit(EXTI_Line1);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
