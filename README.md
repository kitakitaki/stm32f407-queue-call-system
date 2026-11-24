# STM32F407 微机原理课程设计

基于 STM32F407ZET6 的微机原理课程大作业，使用 Keil µVision 5（ARM Compiler 5）开发。

## 功能概览

- LED 流水灯演示
- 呼叫服务流程：通过按键触发服务、数码管显示和蜂鸣器提示
- 基于定时器与外部中断的控制逻辑
- USART 串口输出

主要应用代码位于 `user/`，板级驱动位于 `bsp/`，系统支持代码位于 `system/`。`software_block.png` 展示了模块关系。

## 硬件与工程环境

- MCU：STM32F407ZETx（Cortex-M4）
- IDE：Keil µVision 5
- 工程文件：`test.uvprojx`
- 当前工程配置使用 ARM Compiler 5.06 Update 6

外设引脚和实验板连接方式请以源码注释及课程硬件为准；更换开发板时，先核对 LED、按键、数码管、蜂鸣器和串口的引脚定义。

## 打开与编译

1. 安装 Keil µVision 5 和 STM32F4 系列器件支持包。
2. 在 µVision 中打开 `test.uvprojx`。
3. 确认工程能找到 STM32F407 器件包和 ARM Compiler 5，再执行 Build。
4. 按课程实验板配置下载器与调试接口后烧录运行。

编译输出和 IDE 用户状态文件不纳入版本控制。首次打开工程时，µVision 会在本地重新生成这些文件。

## 目录结构

```text
Bigwork/
├── bsp/           # LED、按键、数码管、蜂鸣器、定时器和外部中断驱动
├── core/          # Cortex-M4 核心头文件与启动文件
├── library/       # STM32 外设库相关文件
├── system/        # 延时、串口和系统支持代码
├── user/          # 主程序与课程设计应用逻辑
├── DebugConfig/   # Keil 调试配置
├── *.png          # 模块图与实验过程示意图
└── test.uvprojx   # Keil µVision 工程文件
```

## 说明

这是课程学习项目，代码按当前实验板配置编写。`library/`、`core/` 中包含 ST/ARM 相关支持文件，其版权及使用条件以各文件自带声明和对应上游许可为准。
