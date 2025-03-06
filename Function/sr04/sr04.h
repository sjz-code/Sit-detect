#ifndef _SR04_H_
#define _SR04_H_

#include <stdio.h>
#include "main.h"

#define		Trigger_ON 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define		Trigger_OFF 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)


void Delay_us(uint16_t us);						// 使用定时器2编写的延时函数
void Trigger_signal(void);						// 发送 10us 的触发信号

extern int distance_cm;



#endif
