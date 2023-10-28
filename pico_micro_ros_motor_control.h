
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

#endif /* PICO_MOTOR_COMTROL_H */
