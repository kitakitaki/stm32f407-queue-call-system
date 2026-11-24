/******************************************************************************
 * switch.c - 开关驱动程序
 * 硬件连接: 开关接在GPIOF Pin0~Pin7，按下为高电平，松开为低电平
 ******************************************************************************/
#include "switch.h"
#include "stm32f4xx.h"

/**
 * @brief  初始化开关所连接的GPIOF引脚 (PF0~PF7)
 *         配置为输入模式，下拉电阻
 */
void Switch_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1
                                | GPIO_Pin_2 | GPIO_Pin_3
                                | GPIO_Pin_4 | GPIO_Pin_5
                                | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   // 输入模式无效
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 输入模式无效
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;   // 下拉，按下为高电平
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/**
 * @brief  读取指定开关的状态
 * @param  pin 开关编号 (0~7)，对应 PF0~PF7
 * @return 0: 松开 (低电平), 1: 按下 (高电平)
 */
uint8_t Switch_Read(uint8_t pin)
{
    if (pin > 7) return 0;
    return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0 << pin);
}

/**
 * @brief  配置S7(PF7)的EXTI中断，下降沿触发(S7由1变0时触发)
 * @note   中断服务函数 EXTI9_5_IRQHandler 需由用户自行编写
 */
void Switch_EXTI_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能SYSCFG时钟（配置EXTI需要） */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* 将EXTI线7连接到GPIOF */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);

    /* 配置EXTI线7：中断模式、下降沿触发 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 清除可能的遗留pending位，防止使能NVIC后立即触发中断 */
    EXTI_ClearITPendingBit(EXTI_Line7);

    /* 配置NVIC中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
