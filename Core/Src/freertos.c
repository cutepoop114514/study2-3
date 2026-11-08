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

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

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
  sin_generate_taHandle = osThreadNew(StartDefaultTask, NULL, &sin_generate_ta_attributes);

  /* creation of motor_task */
  motor_taskHandle = osThreadNew(StartTask02, NULL, &motor_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the sin_generate_ta thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  const float A = 100.0f;                               // 幅值
  const float f = 0.2f;                                 // 频率 0.2Hz (周期 5秒)
  const float omega = 2.0f * 3.14159265f * f;           // 角频率
  static float t = 0.0f;                                
  
  for(;;)
  {
    // 每次加 0.01 秒 (因为任务延时是 10ms)
    t += 0.01f; 
    
    // 【关键点】如果时间超过 5 秒 (一个完整周期)，就减去 5 秒重新开始
    // 这样 t 永远在 0~5 之间循环，不仅一直跑，而且精度永远不会丢失！
    if (t >= 5.0f) {
        t -= 5.0f;
    }
    
    // 计算正弦值
    g_target_speed = (int16_t)(A * sinf(omega * t)); 
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); 
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the motor_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  motor_init(); // 小写
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  for(;;)
  {
    pid_task(); // 调用 PID 控制函数，调节电机速度
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

