#include "main.h"

const uint MOTOR_PIN_1 = 12;
const uint MOTOR_PIN_2 = 13;

int gpio_motor_init(void)
{
    // Initialize stdio for debugging purposes
    stdio_init_all();

    const uint PWM_FREQUENCY = 50; // 50 Hz

    // Initialize the PWM for the first pin
    uint slice1 = pwm_gpio_to_slice_num(MOTOR_PIN_1);
    pwm_config config1 = pwm_get_default_config();
    pwm_set_wrap(slice1, 19999); // for 50Hz with a system clock of 2MHz
    pwm_init(slice1, &config1, true);
    gpio_set_function(MOTOR_PIN_1, GPIO_FUNC_PWM);

    // Initialize the PWM for the second pin
    uint slice2 = pwm_gpio_to_slice_num(MOTOR_PIN_2);
    pwm_config config2 = pwm_get_default_config();
    pwm_set_wrap(slice2, 19999); // for 50Hz with a system clock of 2MHz
    pwm_init(slice2, &config2, true);
    gpio_set_function(MOTOR_PIN_2, GPIO_FUNC_PWM);

    return 1;
}

void motor_speed_set(int L, int R)
{
    // Set the duty cycle for each channel
    pwm_set_gpio_level(MOTOR_PIN_1, 1500); // 1.5ms pulse width
    pwm_set_gpio_level(MOTOR_PIN_2, 1000); // 1.0ms pulse width
    // sleep_ms(20); // Sleep to make it easier to observe, not necessary for real code
}