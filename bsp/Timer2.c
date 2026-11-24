/******************************************************************************
 * Timer2.c - TIM2定时器中断驱动程序
 * 功能: TIM2定时中断初始化及中断服务函数框架
 * 说明: 调用TIM2_Init函数初始化定时器后，用户需在自己的.c文件中声明TIME2_IRQHandler并编写自己的中断处理代码
 ******************************************************************************/
#include "Timer2.h"

/**
 * @brief  初始化TIM2定时器
 * @param  arr: 自动重装载值
 * @param  psc: 时钟预分频值
 * @note   计数时钟 = 84MHz / (psc+1)
 *         定时周期 = (arr+1) * (psc+1) / 84MHz (单位:秒)
 */
void TIM2_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能TIM2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* 配置定时器时基 */
    TIM_TimeBaseStructure.TIM_Prescaler = psc;// 预分频值
    TIM_TimeBaseStructure.TIM_Period = arr;// 自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    //T_out = (arr+1)*(psc+1)/f_clk = (9999+1)*(8399+1)/84MHz = 1s
    
    /* 使能TIM2更新中断 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* 配置NVIC中断优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 使能TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

/*
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        /* 用户在此处编写自己的中断处理代码 
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
*/