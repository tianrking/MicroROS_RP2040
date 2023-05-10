#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Declare global slice numbers for the PWM
uint motor1_slice, motor2_slice, servo_slice;

void setup() {
    // Set the GPIOs to be used as PWM
    gpio_set_function(20, GPIO_FUNC_PWM);
    gpio_set_function(21, GPIO_FUNC_PWM);
    gpio_set_function(19, GPIO_FUNC_PWM);

    // Get the PWM slice numbers for the GPIOs
    motor1_slice = pwm_gpio_to_slice_num(20);
    motor2_slice = pwm_gpio_to_slice_num(21);
    servo_slice = pwm_gpio_to_slice_num(19);

    // Set PWM wrap (resolution)
    pwm_set_wrap(motor1_slice, 0xFFFF); // Set resolution for motor 1
    pwm_set_wrap(motor2_slice, 0xFFFF); // Set resolution for motor 2
    pwm_set_wrap(servo_slice, 0xFFFF); // Set resolution for servo

    // Set PWM frequency
    pwm_set_clkdiv(motor1_slice, 4.0f); // Set frequency for motor 1
    pwm_set_clkdiv(motor2_slice, 4.0f); // Set frequency for motor 2
    pwm_set_clkdiv(servo_slice, 20.0f); // Set frequency for servo to 50Hz

    // Enable the PWMs
    pwm_set_enabled(motor1_slice, true);
    pwm_set_enabled(motor2_slice, true);
    pwm_set_enabled(servo_slice, true);
}

void setMotorSpeed(int motor, float speed) {
    // Speed should be between 0.0 (stop) and 1.0 (full speed)
    uint slice = (motor == 1 ? motor1_slice : motor2_slice);
    uint16_t level = (uint16_t)(speed * (float)0xFFFF);
    pwm_set_gpio_level((motor == 1 ? 20 : 21), level);
}

void setServoAngle(float angle) {
    // Angle should be between 0.0 (0 degrees) and 1.0 (180 degrees)
    uint16_t level = (uint16_t)((angle * 0.5f + 0.5f) * (float)0xFFFF);
    pwm_set_gpio_level(19, level);
}

int main() {
    setup();

    while (true) {
        // Control motors and servo here
        setMotorSpeed(1, 0.5); // Set motor 1 at half speed
        setMotorSpeed(2, 0.5); // Set motor 2 at half speed
        setServoAngle(0.5); // Set servo at 90 degrees (assuming 0.5 corresponds to 90 degrees)

        sleep_ms(1000);
    }
}

