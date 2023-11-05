
#ifndef PICO_MOTOR_COMTROL_H
#define PICO_MOTOR_COMTROL_H

#include <stdio.h>

#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "quadrature_encoder.pio.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>

#include <geometry_msgs/msg/twist.h>
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
void subscription_twist_callback(const void * msgin);
void ppm_callback(uint gpio, uint32_t events);

int gpio_motor_init(void);
void motor_speed_set(int L, int R);

void motor_set_speed(uint gpio, float duty_cycle);
// float mapInputToOutput(int input);

typedef enum{
    STOP,
    FORWARD,
    BACKWARD,
} MOTOR_STATE;

typedef enum
{
    T_NONE,
    T_MOTOR,
    T_SERVO,
} CONTROL_TARGET;

int constrainInput(int input);
float mapInputToOutput(int input, CONTROL_TARGET _ct);

void motor_control(MOTOR_STATE _state);

void servo_init(uint gpio,int frq);
void servo_set_angle(uint gpio, int angle) ;

#endif /* PICO_MOTOR_COMTROL_H */
