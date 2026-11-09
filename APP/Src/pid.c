#include "main.h"
#include "pid.h"
#include "motor.h"
#include "uart.h"

extern volatile int16_t g_target_speed; 
float Kp = 1.025f, Ki = 0.009f, Kd = 0.001f; 
static float Out = 0.0f;
static float Error0 = 0.0f, Error1 = 0.0f, ErrorInt = 0.0f;

void pid_task(void) {
    float Actual = motor_get_speed();
    Error1 = Error0;
    Error0 = g_target_speed - Actual;
    ErrorInt += Error0;
    if (ErrorInt > 500.0f) ErrorInt = 500.0f;
    if (ErrorInt < -500.0f) ErrorInt = -500.0f;

    Out = Kp * Error0 + Ki * ErrorInt + Kd * (Error0 - Error1);

    if (Out > 100.0f) Out = 100.0f;
    if (Out < -100.0f) Out = -100.0f;

    UART_SendData("%d,%d,%d\n", g_target_speed, (int)(Actual * 10), (int)Out);

    if(Out != 0) motor_set_speed((int16_t)Out);
    else motor_stop();
}
//test//