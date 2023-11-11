#include "main.h"

int wrap;

void drv_pwm_init(void){
    // 选择输出 pwm 的引脚 用作控制信号传入L298n 电机驱动并完成初始化
    gpio_set_function(GPIO_motor_L_pwm_A, GPIO_FUNC_PWM);
    uint slice_num_L_pwm_A = pwm_gpio_to_slice_num(GPIO_motor_L_pwm_A);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num_L_pwm_A, wrap);
    pwm_set_enabled(slice_num_L_pwm_A, true);

    gpio_set_function(GPIO_motor_L_pwm_B, GPIO_FUNC_PWM);
    uint slice_num_L_pwm_B = pwm_gpio_to_slice_num(GPIO_motor_L_pwm_B);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num_L_pwm_B, wrap);
    pwm_set_enabled(slice_num_L_pwm_B, true);

    gpio_set_function(GPIO_motor_R_pwm_A, GPIO_FUNC_PWM);
    uint slice_num_R_pwm_A = pwm_gpio_to_slice_num(GPIO_motor_R_pwm_A);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num_R_pwm_A, wrap);
    pwm_set_enabled(slice_num_R_pwm_A, true);

    gpio_set_function(GPIO_motor_R_pwm_B, GPIO_FUNC_PWM);
    uint slice_num_R_pwm_B = pwm_gpio_to_slice_num(GPIO_motor_R_pwm_B);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num_R_pwm_B, wrap);
    pwm_set_enabled(slice_num_R_pwm_B, true);

    gpio_set_function(GPIO_servo_pwm, GPIO_FUNC_PWM);
    uint slice_num_servo_pwm = pwm_gpio_to_slice_num(GPIO_servo_pwm);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num_servo_pwm, wrap);
    pwm_set_clkdiv(slice_num_servo_pwm, 40.0f);
    pwm_set_enabled(slice_num_servo_pwm, true);
}