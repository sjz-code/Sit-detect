/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "sr04.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t warming_flag=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId SR04_DistanceHandle;
osThreadId People_DetectHandle;
osThreadId KEY_DetectHandle;
osThreadId Time_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void SR04_Distance_Entry(void const * argument);
void People_Entry(void const * argument);
void KEY_Entry(void const * argument);
void Time_Entry(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SR04_Distance */
  osThreadDef(SR04_Distance, SR04_Distance_Entry, osPriorityHigh, 0, 128);
  SR04_DistanceHandle = osThreadCreate(osThread(SR04_Distance), NULL);

  /* definition and creation of People_Detect */
  osThreadDef(People_Detect, People_Entry, osPriorityIdle, 0, 128);
  People_DetectHandle = osThreadCreate(osThread(People_Detect), NULL);

  /* definition and creation of KEY_Detect */
  osThreadDef(KEY_Detect, KEY_Entry, osPriorityIdle, 0, 128);
  KEY_DetectHandle = osThreadCreate(osThread(KEY_Detect), NULL);

  /* definition and creation of Time_Task */
  osThreadDef(Time_Task, Time_Entry, osPriorityIdle, 0, 128);
  Time_TaskHandle = osThreadCreate(osThread(Time_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin (DEBUG_LED_GPIO_Port,DEBUG_LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_SR04_Distance_Entry */
/**
* @brief Function implementing the SR04_Distance thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SR04_Distance_Entry */
void SR04_Distance_Entry(void const * argument)
{
  /* USER CODE BEGIN SR04_Distance_Entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(50);
  }
  /* USER CODE END SR04_Distance_Entry */
}

/* USER CODE BEGIN Header_People_Entry */
/**
* @brief Function implementing the People_Detect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_People_Entry */
void People_Entry(void const * argument)
{
  /* USER CODE BEGIN People_Entry */
	uint8_t people_count=0;//检测到次数
	OLED_ShowString(0,0,"Sit_time:",16);
	OLED_ShowString(96,1,"m",8);
	OLED_ShowString(120,1,"s",8);
	OLED_ShowString(0,2,"Sit_Detect:",16);
	OLED_ShowString(96,2,"N",16); 
  /* Infinite loop */
  for(;;)
  {

		if(HAL_GPIO_ReadPin(People_Detect_GPIO_Port,People_Detect_Pin)==GPIO_PIN_RESET)//是否有人靠近
		{
			people_count++;
		}
		else if(HAL_GPIO_ReadPin(People_Detect_GPIO_Port,People_Detect_Pin)==GPIO_PIN_SET)
		{
			people_count=0;
		}
		if(people_count==0)
		{
//			OLED_ShowString(96,0,"Y",16);
			HAL_GPIO_WritePin(People_LED_GPIO_Port,People_LED_Pin,GPIO_PIN_RESET);
		}
		else if(people_count>10)
		{
//			OLED_ShowString(96,0,"N",16);
			HAL_GPIO_WritePin(People_LED_GPIO_Port,People_LED_Pin,GPIO_PIN_SET);
			people_count=20;
		}
		
    osDelay(500);
  }
  /* USER CODE END People_Entry */
}

/* USER CODE BEGIN Header_KEY_Entry */
/**
* @brief Function implementing the KEY_Detect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KEY_Entry */
void KEY_Entry(void const * argument)
{
  /* USER CODE BEGIN KEY_Entry */
  /* Infinite loop */
  for(;;)
  {
	if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==GPIO_PIN_RESET)
	{
    osDelay(10);
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==GPIO_PIN_RESET)
		{
			if(warming_flag==0)
			{
				warming_flag=1;
				
			}
			else if(warming_flag==1)
			{
				warming_flag=0;
			}
		}
		while(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==GPIO_PIN_RESET);
  }
	osDelay(10);
	}
  /* USER CODE END KEY_Entry */
}

/* USER CODE BEGIN Header_Time_Entry */
/**
* @brief Function implementing the Time_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Time_Entry */
void Time_Entry(void const * argument)
{
  /* USER CODE BEGIN Time_Entry */
	uint8_t second,minute=0;
  /* Infinite loop */
  for(;;)
  {
		if(sit_flag==0)
		{
			second++;
			if(second==60)
			{
				minute++;
				second=0;
			}
			if(minute==60)
			{
				minute=0;
			}
			OLED_ShowString(96,2,"Y",16); 
		}
		else
		{
			second=0;
			minute=0;
			OLED_ShowString(96,2,"N",16); 
			
		}
		OLED_ShowNum(80,1,minute/10,1,8);
		OLED_ShowNum(88,1,minute%10,1,8);
		OLED_ShowNum(104,1,second/10,1,8);
		OLED_ShowNum(112,1,second%10,1,8);
		if((minute>=1)&&(warming_flag==0))
		{
			HAL_GPIO_WritePin(Warming_LED_GPIO_Port,Warming_LED_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(Warming_LED_GPIO_Port,Warming_LED_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
		}
    osDelay(1000);
  }
  /* USER CODE END Time_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

