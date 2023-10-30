#include "main.h"

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

void motorA_gpio_init()
{
    gpio_init(AIN1);
    gpio_init(AIN2);
    gpio_set_dir(AIN1, GPIO_OUT);
    gpio_set_dir(AIN2, GPIO_OUT);
}
