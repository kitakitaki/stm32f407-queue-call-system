/******************************************************************************
 * beep.h - 蜂鸣器驱动
 * 硬件连接: 无源蜂鸣器接PD14，需输入脉冲产生声音
 ******************************************************************************/
#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f4xx.h"

/**
 * @brief  初始化蜂鸣器引脚PD14为推挽输出
 */
void BEEP_Init(void);

/**
 * @brief  让蜂鸣器发声
 * @param  duration_ms 发声持续时间(ms)
 * @param  freq 音调频率(Hz)，如 440=La4, 523=Do5
 * @param  volume 音量百分比 (0~100)，控制占空比
 *         0=静音, 50=中等, 100=最大音量(50%占空比)
 * @note   占空比 = volume / 2
 *         频率越高音调越高
 */
void BEEP_Sound(uint16_t duration_ms, uint16_t freq, uint8_t volume);

#endif /* __BEEP_H */
