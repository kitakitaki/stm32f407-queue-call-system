#include "call_service.h"
#include <stdio.h>

/* 引用 switch_call.c 中的系统状态变量 */
extern uint8_t takenum_state;   // 0:停止取号, 1:接受取号

/* ======================== 窗口状态定义 ======================== */
typedef enum {
    WIN_READY,      // Ln亮，等待分配排队序号
    WIN_CALLING,    // Ln以1Hz闪烁，等待顾客到达
    WIN_SERVING     // Ln灭，业务办理中
} WinState_t;

/* ======================== 全局变量 ======================== */
static uint8_t g_max_ticket = 0;            // 当前最大排队序号 (0~15)
static uint8_t g_next_to_serve = 1;         // 下一个待分配序号

static WinState_t g_win_state[3] = {WIN_READY, WIN_READY, WIN_READY}; // 窗口4,5,6

/* ======================== 查找空闲窗口 ======================== */
static int8_t FindReadyWindow(void)
{
    for (int i = 0; i < 3; i++)
        if (g_win_state[i] == WIN_READY) return i;
    return -1;
}

/* ======================== 判断所有窗口是否空闲 ======================== */
static uint8_t AllWindowsReady(void)
{
    for (int i = 0; i < 3; i++)
        if (g_win_state[i] != WIN_READY) return 0;
    return 1;
}

/* ======================== 尝试叫号 ======================== */
static void CallNext(void)
{
    if (g_next_to_serve > g_max_ticket) return;     // 无待分配序号
    if (g_max_ticket == 0) return;                  // 尚未取号

    int8_t win_idx = FindReadyWindow();
    if (win_idx < 0) return;                        // 无空闲窗口

    uint8_t win_n = 4 + win_idx;                    // 窗口编号 4,5,6
    uint8_t queue_x = g_next_to_serve;              // 排队序号

    g_win_state[win_idx] = WIN_CALLING;
    g_next_to_serve++;

    // USART 串口叫号
    printf("Custom#%d to Window#%d\r\n", queue_x, win_n);
}

/* ======================== 窗口状态处理 ======================== */
static void ProcessWindows(void)
{
    for (int i = 0; i < 3; i++)
    {
        uint8_t win_n = 4 + i;

        switch (g_win_state[i])
        {
            case WIN_CALLING:
                // Sn拨向上方(高电平=1) ， 顾客到达
                if (Switch_Read(win_n) == 1)
                {
                    LED_OFF(win_n);             // Ln熄灭，业务办理中
                    g_win_state[i] = WIN_SERVING;
                }
                break;

            case WIN_SERVING:
                // Sn拨向下方(低电平=0) → 业务完毕，顾客离开
                if (Switch_Read(win_n) == 0)
                {
                    LED_ON(win_n);              // Ln点亮，窗口就绪
                    g_win_state[i] = WIN_READY;
                }
                break;

            default:
                break;
        }
    }
}

/* ======================== TIM3中断: LED闪烁控制 ======================== */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        static uint8_t blink = 0;
        blink = !blink;

        for (int i = 0; i < 3; i++)
        {
            if (g_win_state[i] == WIN_CALLING)
            {
                if (blink)
                    LED_OFF(4 + i);     // 灭
                else
                    LED_ON(4 + i);      // 亮
            }
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

/* ======================== EXTI1中断: 取号 ======================== */
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        if (takenum_state && g_max_ticket < 15) // 系统接受取号且未达上限
        {
            g_max_ticket++;
            Digitron_Display(0, g_max_ticket);  // 数码管显示当前最大序号(十六进制)
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

/* ======================== 主服务函数 ======================== */
void call_service(void)
{
    Digitron_Init();
    Digitron_Display(0, 0); // 第1个数码管显示0，表示第1个叫号

    // 点亮LED4、5、6，表示系统待命
    LED_ON(4);
    LED_ON(5);
    LED_ON(6);

    // 等待S4、S5、S6全部拨向下方（低电平=0）
    while (Switch_Read(4) || Switch_Read(5) || Switch_Read(6))
    {
        // 只要任一个在(高电平=1)，继续等待
    }

    // 初始化串口(波特率115200)，用于叫号输出
    uart_init(115200);
    printf("System Ready.\r\n");

    // 初始化TIM3(1Hz闪烁)
    TIM3_Init();

    // 初始化EXTI1(取号按键PA1)，下降沿触发
    EXTI1_PA1_Init();


    // 主循环: 窗口状态处理 + 叫号
    // 退出条件: S7已关闭(停止取号) && 所有序号已分配 && 所有窗口空闲(顾客服务完毕)
    while (takenum_state || g_next_to_serve <= g_max_ticket || !AllWindowsReady())
    {
        ProcessWindows();       // 根据状态更新窗口的LED显示
        CallNext();             // 有窗口空闲且有排队序号时叫号
        delay_ms(50);           // 避免过度占用CPU
    }

    // 退出前熄灭窗口LED
    LED_OFF(4);
    LED_OFF(5);
    LED_OFF(6);
}