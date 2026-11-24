#ifndef __SWITCH_CALL_H
#define __SWITCH_CALL_H

#include "stm32f4xx.h"
#include "switch.h"
#include "led.h"
#include "call_service.h"


extern uint8_t takenum_state; // 0: 不接受叫号, 1: 接受叫号
extern uint8_t service_state; // 0：不服务，1：接受服务已取号者
void switch_call(void);

#endif