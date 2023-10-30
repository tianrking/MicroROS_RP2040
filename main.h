
#ifndef PICO_MOTOR_COMTROL_H
#define PICO_MOTOR_COMTROL_H

#include <stdio.h>

#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "quadrature_encoder.pio.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float64.h>
#include <std_msgs/msg/string.h>
#include <rmw_microros/rmw_microros.h>

#include "pico/stdlib.h"
#include "pico_uart_transports.h"

#include "drv/motor_control.h"
#include "drv/remote_ppm.h"
#include "drv/debug_module.h"
#include "drv/cooneo_motor_control.h"
#include "drv/pwm_module.h"

float moving_average(float new_value, float window[], int *index);
int caculate(int now, int target);

uint16_t read_adc(uint channel) ;

void timer_callback(rcl_timer_t *timer, int64_t last_call_time);
void gpio_2_callback(uint gpio, uint32_t events);
void ppm_callback(uint gpio, uint32_t events);

void subscription_callback_speed_change(const void *msgin_diy);

void ppm_callback(uint gpio, uint32_t events);

int gpio_motor_init(void);
void motor_speed_set(int L, int R);

// Wait for agent successful ping for 2 minutes.
#define timeout_ms 1000
#define attempts 120

#define GPIO_motor_L_pwm_A 6
#define GPIO_motor_L_pwm_B 7
#define GPIO_motor_R_pwm_A 8
#define GPIO_motor_R_pwm_B 9
#define GPIO_servo_pwm 5

#endif /* PICO_MOTOR_COMTROL_H */
