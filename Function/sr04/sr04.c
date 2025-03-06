#include "sr04.h"

int distance_cm = 0;


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


/* 延时函数（单位 us） */
void Delay_us(uint16_t us)
{
	uint16_t time = 0xffff - us - 5;
	__HAL_TIM_SET_COUNTER(&htim2,time);								// 设置 定时器2 的值
	HAL_TIM_Base_Start(&htim2);										// 开启定时器2

	while(time < 0xffff-5)
	{
		time = __HAL_TIM_GET_COUNTER(&htim2);						// 获取定时器2 值
	}

	HAL_TIM_Base_Stop(&htim2);										// 停止 定时器2
}


/* 发送 10us 的触发信号 */
void Trigger_signal(void)
{
	Trigger_ON;							// 发送高电平
	Delay_us(10);						// 延时 10 us
	Trigger_OFF;						// 发送低电平
}


/* 外部中断回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	 static uint32_t time_us = 0;
	
	if(GPIO_Pin == GPIO_PIN_10)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
		{
			HAL_TIM_Base_Start(&htim3);									// 开启定时器3（开始计时）
			__HAL_TIM_SetCounter(&htim3,0);								// 清空定时器3
		}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10) == 0)
		{
			HAL_TIM_Base_Stop(&htim3);									//关闭定时器3（停止计时）
			time_us = __HAL_TIM_GetCounter(&htim3);							// 获取高电平时间
//			printf("time_us : %d\r\n", time_us);
			distance_cm = time_us * 340/2*0.000001*100;
//			printf("distance_cm is %d cm\r\n", distance_cm);
			
			time_us = 0;
		}
	}
}
