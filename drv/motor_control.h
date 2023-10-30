#ifndef DRV_PICO_MOTOR_COMTROL_H
#define DRV_PICO_MOTOR_COMTROL_H

int gpio_motor_init(void);
void motor_speed_set(int L, int R);

#endif