/******************************************************************************
 * digitron.c - 数码管驱动 (74HC595)
 * 硬件连接:
 *   DS (数据)    → PD5
 *   SH_CP (移位) → PD7
 *   ST_CP (锁存) → PD6
 *   OE (使能)    → PD12 (低电平有效)
 *   两个级联74HC595: U7=位选, U8=段选
 *   8位共阴极数码管
 ******************************************************************************/
#include "digitron.h"

// 共阴极数码管段码表 (0~9, A~F 十六进制)
static const uint8_t seg_table[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A (10)
    0x7C, // b (11)
    0x39, // C (12)
    0x5E, // d (13)
    0x79, // E (14)
    0x71  // F (15)
};

// 位选表 (共阴极，低电平选中对应数码管)
static const uint8_t bit_sel_table[] = {
    0xFE, // 第1个 (1111 1110)
    0xFD, // 第2个 (1111 1101)
    0xFB, // 第3个 (1111 1011)
    0xF7, // 第4个 (1111 0111)
    0xEF, // 第5个 (1110 1111)
    0xDF, // 第6个 (1101 1111)
    0xBF, // 第7个 (1011 1111)
    0x7F  // 第8个 (0111 1111)
};

/**
 * @brief  通过两个级联的74HC595发送16位数据
 *         先发高8位(位选→U7)，后发低8位(段选→U8)
 * @param  bit_sel 位选数据
 * @param  seg_sel 段选数据
 */
static void HC595_WriteTwoBytes(uint8_t bit_sel, uint8_t seg_sel)
{
    uint16_t data = ((uint16_t)bit_sel << 8) | seg_sel;

    for (int i = 0; i < 16; i++) {
        // MSB first
        if (data & 0x8000)
            GPIO_SetBits(GPIOD, GPIO_Pin_5);    // DS = 1
        else
            GPIO_ResetBits(GPIOD, GPIO_Pin_5);  // DS = 0

        // SH_CP 上升沿，移入一位
        GPIO_SetBits(GPIOD, GPIO_Pin_7);
        GPIO_ResetBits(GPIOD, GPIO_Pin_7);

        data <<= 1;
    }

    // ST_CP 上升沿，锁存输出到 U7 和 U8
    GPIO_SetBits(GPIOD, GPIO_Pin_6);
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);
}

/**
 * @brief  初始化74HC595所连接的GPIOD引脚 (PD5~PD7, PD12)
 *         PD5=DS, PD6=ST_CP, PD7=SH_CP, PD12=OE
 */
void Digitron_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6
                                | GPIO_Pin_7 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // OE 拉低，使能输出 (OE低电平有效)
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}

/**
 * @brief  在指定数码管上显示数字 (十六进制 0~F)
 * @param  pos 数码管位置 (0~7，对应第1~8个数码管)
 * @param  num 要显示的数字 (0~15，10~15 显示为 A~F)
 */
void Digitron_Display(uint8_t pos, uint8_t num)
{
    if (pos > 7 || num > 15) return;
    HC595_WriteTwoBytes(bit_sel_table[pos], seg_table[num]);
}
