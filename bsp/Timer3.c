/******************************************************************************
 * Timer3.c - TIM3定时器驱动程序
 * 功能: TIM3定时中断初始化
 * 说明: 初始化后，用户需自行实现 TIM3_IRQHandler 中断服务函数
 *       默认定时周期0.5s，可用于1Hz闪烁控制
 ******************************************************************************/
#include "Timer3.h"

/**
 * @brief  初始化TIM3定时器，周期0.5s
 * @note   计数时钟 = 84MHz / (8399+1) = 10KHz
 *         定时周期 = (4999+1) / 10KHz = 0.5s
 *         中断服务函数 TIM3_IRQHandler 需用户自行编写
 */
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 定时周期: (8399+1)*(4999+1)/84MHz = 0.5s */
    TIM_TimeBaseStructure.TIM_Prescaler = 8399;
    TIM_TimeBaseStructure.TIM_Period = 4999;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}
