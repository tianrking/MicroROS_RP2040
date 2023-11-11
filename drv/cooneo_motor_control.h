#ifndef DRV_COONEO_MOTOR_CONTROL_H
#define DRV_COONEO_MOTOR_CONTROL_H

#define CAR_WIDTH    (0.075)  //m
#define CAR_REAR_WHEEL_R (0.03)  //m
#define SPEED2RPM (1 / ((CAR_REAR_WHEEL_R) * 2 * M_PI) * 60)
#define RPM2SPEED (((CAR_REAR_WHEEL_R) * 2 * M_PI) / 60) 
#define INVERSE_K 1
// Pin definitions
#define AIN1  3
#define AIN2  2
#define PWMA  12

// #define AIN1  27
// #define AIN2  28
// #define PWMA  21 

#define BIN2 10 // switch BIN2 BIN1 mirror install
#define BIN1 11
#define PWMB 13

// #define BIN2 19 // switch BIN2 BIN1 mirror install
// #define BIN1 18
// #define PWMB 20

//CIN1 25 24  PWMC26 

//DIN1 32 34  PWMD27

// #define BIN2 27 // switch BIN2 BIN1 mirror install
// #define BIN1 28
// #define PWMB 21

// #define BIN2 3 // switch BIN2 BIN1 mirror install
// #define BIN1 2
// #define PWMB 12

void setup_pwm(uint gpio, float duty_cycle);
void motorA_forward(void);
void motorA_backward(void);
void motorA_stop(void);
void motorA_gpio_init(void);
void motorB_forward(void);
void motorB_backward(void);
void motorB_stop(void);
void motorB_gpio_init(void);
void calculate_wheel_speeds(double v, double w, double wheelbase, double *v_left, double *v_right);
double map_value(double x, double a, double b, double c, double d) ;
void cooneo_inverse_2WD(double w,double v,double *vRL,double *vRR);
#endif