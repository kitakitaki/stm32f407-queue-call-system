/******************************************************************************
 * led.c - LED驱动程序
 * 硬件连接: LED接在GPIOE Pin0~Pin7，低电平点亮
 ******************************************************************************/
#include "led.h"

/**
 * @brief  LED GPIO初始化
 *         配置GPIOE Pin0~Pin7为推挽输出模式
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* 初始化所有LED为熄灭状态（高电平） */
    GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
                      | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

/**
 * @brief  控制指定LED亮灭（低电平点亮）
 * @param  num: LED编号，范围0~7，对应PE0~PE7
 * @note   通过switch-case选择要点亮的LED
 */
void LED_ON(u8 num)
{

    /* 点亮指定LED（低电平有效） */
    switch (num)
    {
        case 0: GPIO_ResetBits(GPIOE, GPIO_Pin_0); break;
        case 1: GPIO_ResetBits(GPIOE, GPIO_Pin_1); break;
        case 2: GPIO_ResetBits(GPIOE, GPIO_Pin_2); break;
        case 3: GPIO_ResetBits(GPIOE, GPIO_Pin_3); break;
        case 4: GPIO_ResetBits(GPIOE, GPIO_Pin_4); break;
        case 5: GPIO_ResetBits(GPIOE, GPIO_Pin_5); break;
        case 6: GPIO_ResetBits(GPIOE, GPIO_Pin_6); break;
        case 7: GPIO_ResetBits(GPIOE, GPIO_Pin_7); break;
        default: break; /* 无效编号，所有LED保持熄灭 */
    }
}
void LED_OFF(u8 num)
{
    /* 熄灭指定LED（高电平有效） */
    switch (num)
    {
        case 0: GPIO_SetBits(GPIOE, GPIO_Pin_0); break;
        case 1: GPIO_SetBits(GPIOE, GPIO_Pin_1); break;
        case 2: GPIO_SetBits(GPIOE, GPIO_Pin_2); break;
        case 3: GPIO_SetBits(GPIOE, GPIO_Pin_3); break;
        case 4: GPIO_SetBits(GPIOE, GPIO_Pin_4); break;
        case 5: GPIO_SetBits(GPIOE, GPIO_Pin_5); break;
        case 6: GPIO_SetBits(GPIOE, GPIO_Pin_6); break;
        case 7: GPIO_SetBits(GPIOE, GPIO_Pin_7); break;
        default: break; /* 无效编号，所有LED保持熄灭 */
    }
}

