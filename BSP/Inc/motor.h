#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

void motor_init(void);
void motor_set_speed(int speed);
void motor_stop(void);
float motor_get_speed(void);

#endif