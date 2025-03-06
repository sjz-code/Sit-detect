#ifndef _SR04_H_
#define _SR04_H_

#include <stdio.h>
#include "main.h"

#define		Trigger_ON 		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define		Trigger_OFF 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)


void Delay_us(uint16_t us);						// ʹ�ö�ʱ��2��д����ʱ����
void Trigger_signal(void);						// ���� 10us �Ĵ����ź�

extern int distance_cm;



#endif
