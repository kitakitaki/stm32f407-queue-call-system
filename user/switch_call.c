// S7=1时，L7点亮，开启等待叫号功能
#include "switch_call.h"
#include "running_LED.h"

uint8_t takenum_state = 0; // 0: 不接受叫号, 1: 接受叫号
uint8_t service_state = 0; // 0：不服务，1：接受服务已取号者
// 下降沿触发中断，S7由1变0时触发
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        LED_OFF(7); // S7关闭，熄灭L7
        // 停止接受取号，只完成剩余服务
        takenum_state = 0;
        service_state = 1;

        EXTI_ClearITPendingBit(EXTI_Line7); // 清除中断标志
    }
}

// 初始化开关和LED, 负责管理叫号窗口的开放和关闭
void switch_call(void)
{

    LED_Init();
    Switch_Init();
    while(1){
        if (Switch_Read(7) == 1) // S7=1，开启叫号服务
        {
            takenum_state = 1; // 接受叫号
            service_state = 1; // 开始服务
            LED_ON(7); // S7打开，点亮L7
            break; // 进入叫号服务主循环
        }
    }
    
    Switch_EXTI_Init(); // 配置S7的EXTI中断（需在call_service之前，否则无限循环）

    if (takenum_state)
    {
        call_service(); // 服务完剩余顾客后返回
        EndServiceAnimation(); // 结束营业动画
    }

}