/******************************************************************************
 * beep.c - 蜂鸣器驱动
 * 硬件连接: 无源蜂鸣器接PD14，需输入脉冲产生声音
 * 说明: 通过PD14输出约2kHz的方波驱动无源蜂鸣器
 ******************************************************************************/
#include "beep.h"
#include "delay.h"

/**
 * @brief  初始化蜂鸣器引脚PD14为推挽输出，初始低电平
 */
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_14); // 初始低电平
}

/**
 * @brief  让蜂鸣器发声
 * @param  duration_ms 发声持续时间(ms)
 * @param  freq 音调频率(Hz)，如 440=La4, 523=Do5
 * @param  volume 音量百分比 (0~100)，控制占空比
 * @note   周期 = 1,000,000 / freq (us)
 *         占空比 = volume / 2
 *         高电平时间 = 周期 * volume / 200
 */
void BEEP_Sound(uint16_t duration_ms, uint16_t freq, uint8_t volume)
{
    if (volume > 100) volume = 100;
    if (freq == 0) return;

    uint32_t period_us = 1000000 / freq;         // 一个周期多少us
    uint32_t high_us   = period_us * volume / 200; // 高电平时间
    uint32_t low_us    = period_us - high_us;     // 低电平时间
    uint32_t cycles    = (uint32_t)duration_ms * 1000 / period_us; // 所需周期数

    delay_init(168);
    for (uint32_t i = 0; i < cycles; i++)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_14);
        delay_us(high_us);
        GPIO_ResetBits(GPIOD, GPIO_Pin_14);
        delay_us(low_us);
    }
    GPIO_ResetBits(GPIOD, GPIO_Pin_14); // 结束后确保低电平
}
