#include "main.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void setup_pwm(uint gpio, float duty_cycle)
{
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 255);
    pwm_set_gpio_level(gpio, duty_cycle * 255);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    pwm_set_enabled(slice_num, true);
}

void motorA_forward()
{
    gpio_put(AIN1, 1);
    gpio_put(AIN2, 0);
}

void motorA_backward()
{
    gpio_put(AIN1, 0);
    gpio_put(AIN2, 1);
}

void motorA_stop()
{
    gpio_put(AIN1, 0);
    gpio_put(AIN2, 0);
}

void motorB_forward()
{
    gpio_put(BIN1, 1);
    gpio_put(BIN2, 0);
}

void motorB_backward()
{
    gpio_put(BIN1, 0);
    gpio_put(BIN2, 1);
}

void motorB_stop()
{
    gpio_put(BIN1, 0);
    gpio_put(BIN2, 0);
}

void motorA_gpio_init()
{
    gpio_init(AIN1);
    gpio_init(AIN2);
    gpio_set_dir(AIN1, GPIO_OUT);
    gpio_set_dir(AIN2, GPIO_OUT);
}

void motorB_gpio_init()
{
    gpio_init(BIN1);
    gpio_init(BIN2);
    gpio_set_dir(BIN1, GPIO_OUT);
    gpio_set_dir(BIN2, GPIO_OUT);
}

void calculate_wheel_speeds(double v, double w, double wheelbase, double *v_left, double *v_right) {
    *v_left = v - (w * wheelbase) / 2.0;
    *v_right = v + (w * wheelbase) / 2.0;
}

double map_value(double x, double a, double b, double c, double d) {
    return ((x - a) / (b - a)) * (d - c) + c;
}

void cooneo_inverse_2WD(double v,double w,double *vRL,double *vRR){
    // *vRL = ( v - w * CAR_WIDTH / 2.0f * ) * SPEED2RPM * INVERSE_K;
    // *vRR = ( v + w * CAR_WIDTH / 2.0f * ) * SPEED2RPM * INVERSE_K;
    *vRL = ( v - w  );
    *vRR = ( v + w  );
}
// int main() {
//     double v = 1.0; // Linear velocity in meters per second
//     double w = 0.5; // Angular velocity in radians per second
//     double wheelbase = 0.5; // Distance between wheels in meters

//     double v_left, v_right;
//     calculate_wheel_speeds(v, w, wheelbase, &v_left, &v_right);

//     printf("Left Wheel Velocity: %f m/s\n", v_left);
//     printf("Right Wheel Velocity: %f m/s\n", v_right);

//     return 0;
// }

// Pin definitions COONEO MOTOR
// const uint AIN1 = 3;
// const uint AIN2 = 2;
// const uint PWMA = 12;

// const uint BIN2 = 10; // switch BIN2 BIN1 mirror install
// const uint BIN1 = 11;
// const uint PWMB = 13;