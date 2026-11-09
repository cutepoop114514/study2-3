/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "uart.h"
#include "motor.h"
#include "pid.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile int16_t g_target_speed = 0; // 目标速度，单位 RPM
extern float Kp, Ki, Kd; // PID 参数
/* USER CODE END Variables */
/* Definitions for sin_generate_ta */
osThreadId_t sin_generate_taHandle;
const osThreadAttr_t sin_generate_ta_attributes = {
  .name = "sin_generate_ta",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for motor_task */
osThreadId_t motor_taskHandle;
const osThreadAttr_t motor_task_attributes = {
  .name = "motor_task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Sin_generate_task(void *argument);
void Motor_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of sin_generate_ta */
  sin_generate_taHandle = osThreadNew(Sin_generate_task, NULL, &sin_generate_ta_attributes);

  /* creation of motor_task */
  motor_taskHandle = osThreadNew(Motor_task, NULL, &motor_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Sin_generate_task */
/**
  * @brief  Function implementing the sin_generate_ta thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Sin_generate_task */
void Sin_generate_task(void *argument)
{
  /* USER CODE BEGIN Sin_generate_task */
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  const float A = 100.0f;                          
  const float f = 0.2f;                                
  const float omega = 2.0f * 3.14159265f * f;          
  static float t = 0.0f;    
  /* Infinite loop */
  for(;;)
  {
    t += 0.01f; 
    if (t >= 5.0f) {
        t -= 5.0f;
    }

    g_target_speed = (int16_t)(A * sinf(omega * t)); 
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); 
  }
  /* USER CODE END Sin_generate_task */
}

/* USER CODE BEGIN Header_Motor_task */
/**
* @brief Function implementing the motor_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Motor_task */
void Motor_task(void *argument)
{
  /* USER CODE BEGIN Motor_task */
  motor_init();
  TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    pid_task();
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
  }
  /* USER CODE END Motor_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

