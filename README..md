# STM32 电机闭环控制

## 项目简介
基于 STM32F103C8T6 + FreeRTOS + CMake 实现的无刷/有刷电机速度闭环控制。
- 使用 TIM2 硬件正交解码读取编码器。
- 使用 TIM4 输出 PWM 控制电机。
- 运行 FreeRTOS 双任务：正弦波发生器 (10ms) + PID 控制器 (5ms)。

## 硬件接线说明
| STM32 引脚 | 外设连接 |
| :--- | :--- |
| PA0 (TIM2_CH1) | 编码器 A 相 |
| PA1 (TIM2_CH2) | 编码器 B 相 |
| PB8 (TIM4_CH3) | 电机驱动 PWM |
| PA5 | 电机驱动 AIN1 |
| PA6 | 电机驱动 AIN2 |
| PA2 / PA3 | USART2 TX / RX (连接串口模块) |

## 编译与烧录
1. 环境：VSCode + CMake + arm-none-eabi-gcc
2. 终端执行：
   ```bash
   cmake -B build -G "Ninja"
   cmake --build build

使用说明

· 上电后，电机将跟随 0.2Hz 的正弦波目标速度转动。
· 串口波特率 115200，VOFA+ 选择 FireWater 协议。
· 在线调参：在串口发送 0.5,0.1,0 即可修改 Kp, Ki, Kd。