#include "stm32f4xx.h"
#include "running_LED.h"
#include "switch_call.h"

int main(void)
{
	running_LED();      // 流水灯演示（完成后TIM2自动关闭）
	switch_call();      // 进入叫号服务系统（S7控制开关）
}
/* 代码结构:
main()
  ├── running_LED()         ← 流水灯演示（只用TIM2+LED）
  │
  └── switch_call()
        ├── LED_Init()
        ├── Switch_Init()
        ├── while(1) 等待 S7=1    ← 蜂鸣器未初始化，不会响
        │
        ├── S7=1 → LED_ON(7)
        ├── Switch_EXTI_Init()
        │
        └── call_service()
              ├── 服务中...
              ├── S7下降沿 → 停止取号
              ├── 服务完剩余顾客
              └── 返回 → EndServiceAnimation()
                           └── BEEP_Init()   蜂鸣器初始化
                           └── LED7~0闪烁 + 蜂鸣器响 
*/