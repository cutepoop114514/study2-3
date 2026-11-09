#include "main.h"
#include "tim.h"
#include "motor.h"
#include "stm32f1xx_hal_tim.h"

void motor_init(void) {
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); 
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); 
    
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

void motor_set_speed(int speed) {

    uint16_t pwm = (speed > 0 ? speed : -speed); 
    if (pwm > 99) pwm = 99;

    if (speed >= 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pwm); 
    } else {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pwm); 
    }
}

void motor_stop(void) {
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

float motor_get_speed(void) {
    int16_t temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    float temp1 = (float)temp * 700.0f / 1000.0f; 
    static float speed = 0;
    speed = 0.75f * speed + 0.25f * temp1;
    return speed; 
}