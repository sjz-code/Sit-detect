#include "sr04.h"

int distance_cm = 0;


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


/* ��ʱ��������λ us�� */
void Delay_us(uint16_t us)
{
	uint16_t time = 0xffff - us - 5;
	__HAL_TIM_SET_COUNTER(&htim2,time);								// ���� ��ʱ��2 ��ֵ
	HAL_TIM_Base_Start(&htim2);										// ������ʱ��2

	while(time < 0xffff-5)
	{
		time = __HAL_TIM_GET_COUNTER(&htim2);						// ��ȡ��ʱ��2 ֵ
	}

	HAL_TIM_Base_Stop(&htim2);										// ֹͣ ��ʱ��2
}


/* ���� 10us �Ĵ����ź� */
void Trigger_signal(void)
{
	Trigger_ON;							// ���͸ߵ�ƽ
	Delay_us(10);						// ��ʱ 10 us
	Trigger_OFF;						// ���͵͵�ƽ
}


/* �ⲿ�жϻص����� */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	 static uint32_t time_us = 0;
	
	if(GPIO_Pin == GPIO_PIN_10)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
		{
			HAL_TIM_Base_Start(&htim3);									// ������ʱ��3����ʼ��ʱ��
			__HAL_TIM_SetCounter(&htim3,0);								// ��ն�ʱ��3
		}
		else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10) == 0)
		{
			HAL_TIM_Base_Stop(&htim3);									//�رն�ʱ��3��ֹͣ��ʱ��
			time_us = __HAL_TIM_GetCounter(&htim3);							// ��ȡ�ߵ�ƽʱ��
//			printf("time_us : %d\r\n", time_us);
			distance_cm = time_us * 340/2*0.000001*100;
//			printf("distance_cm is %d cm\r\n", distance_cm);
			
			time_us = 0;
		}
	}
}
