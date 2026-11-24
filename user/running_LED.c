#include "running_LED.h"
#include "beep.h"

void TIM2_IRQHandler(void)
{
    static int8_t ledx = 7; // 当前要处理的LED编号 (0~7)
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        if (ledx >= 0)
        {
            if (ledx < 8)
                LED_OFF(ledx + 1);  // 关闭上一个LED
            LED_ON(ledx);           // 点亮当前LED
            ledx--;
        }

        if (ledx < 0)
        {
            delay_init(168);
            delay_ms(500);          // 等待0.5秒
            LED_OFF(0);             // 关闭最后一个LED
            /* 8个LED已全部点亮，关闭TIM2更新中断 */
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void running_LED(void)
{
    LED_Init();
    TIM2_Init(4999, 8399); // 0.5s定时中断
}

/**
 * @brief  结束服务动画
 *         LED7~LED0以2Hz依次点亮，然后蜂鸣器播放旋律
 */
void EndServiceAnimation(void)
{
    // LED7 ~ LED0 依次点亮，每个0.5s (2Hz)
    for (int8_t i = 0; i <= 7; i++)
    {
        LED_ON(i);
        delay_ms(500);
        LED_OFF(i);
    }                                         

    // 蜂鸣器播放结束旋律
    BEEP_Init();
    // 乐谱: {频率(Hz), 时长(ms)}，0=休止符
    // A3=220, B3=247, C4=262, D4=294, E4=330, F4=349, G4=392, A4=440
    const uint16_t melody[][2] = {
        {220,300}, {330,150}, {330,150}, {330,150}, {330,150},
        {262,300}, {294,150}, {262,150},
        {247,300}, {220,300},
        {0,  300},
        {440,150}, {440,150}, {440,150}, {440,150}, {440,300},
        {330,300},
        {392,150}, {392,150},
        {349,300}, {330,300},
    };

    uint8_t total = sizeof(melody) / sizeof(melody[0]);
    for (uint8_t i = 0; i < total; i++)
    {
        if (melody[i][0] > 0)
            BEEP_Sound(melody[i][1], melody[i][0], 8);
        else
            delay_ms(melody[i][1]);
    }
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
}