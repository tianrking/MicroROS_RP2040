#ifndef DRV_COONEO_MOTOR_CONTROL_H
#define DRV_COONEO_MOTOR_CONTROL_H

// Pin definitions
#define AIN1  3
#define AIN2  2
#define PWMA  12

void setup_pwm(uint gpio, float duty_cycle);
void motorA_forward(void);
void motorA_backward(void);
void motorA_stop(void);
void motorA_gpio_init(void);

#endif