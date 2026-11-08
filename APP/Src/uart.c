#include "uart.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern float Kp, Ki, Kd; // 引用 pid.c 里的参数

void UART_SendData(const char *format, ...) {
    static char buffer[128]; // 必须是 static
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (huart2.gState == HAL_UART_STATE_READY) {
        HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buffer, strlen(buffer));
    }
}

// 串口接收中断回调函数 (用于调参)
uint8_t rx_byte;
char rx_buffer[64];
int rx_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rx_byte == '\n' || rx_byte == '\r') {
            rx_buffer[rx_index] = '\0';
            if (rx_index > 0) {
                sscanf(rx_buffer, "%f,%f,%f", &Kp, &Ki, &Kd);
            }
            rx_index = 0;
        } else {
            rx_buffer[rx_index++] = rx_byte;
            if (rx_index >= 63) rx_index = 0;
        }
        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
    }
}