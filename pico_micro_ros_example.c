#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int8.h>

#include <rmw_microros/rmw_microros.h>

#include "pico/stdlib.h"
#include "pico_wifi_transport.h"


#include "main.h"

// #define CAR_WIDTH 75  //mm
int new_value, delta, old_value = 0;
const uint sm = 0;
PIO pio = pio0;

const uint LED_PIN = 25;
// Pin definitions COONEO MOTOR
// const uint AIN1 = 3;
// const uint AIN2 = 2;
// const uint PWMA = 12;

// const uint BIN2 = 10; // switch BIN2 BIN1 mirror install
// const uint BIN1 = 11;
// const uint PWMB = 13;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rcl_publisher_t publisher_move_x;
std_msgs__msg__Float64 msg_move_x;

rcl_publisher_t publisher_move_y;
std_msgs__msg__Float64 msg_move_y;

rcl_publisher_t publisher_encoder;
std_msgs__msg__Int32 msg_publisher_encoder;
std_msgs__msg__String msg_publisher_encoder_String;

rcl_subscription_t subscriber_speed_change;
std_msgs__msg__Int32 msg_subscriber_speed_change;

rcl_subscription_t subscriber_angle_change;
std_msgs__msg__Int32 msg_subscriber_angle_change;

rcl_publisher_t publisher_encoder_move_x;
std_msgs__msg__Int32 msg_publisher_move_x;
rcl_publisher_t publisher_encoder_move_y;
std_msgs__msg__Int32 msg_publisher_move_y;

int vv,ww;
double vRR,vRL;
float output_pwm = 0; // 12000
float kp = 5, ki = 0.4, kd = 0;
float err, last_err;
float err_i;
float err_d;
float fix;

#define WINDOW_SIZE 20
float window_2[WINDOW_SIZE] = {0};
int index_window_2 = 0;
float window_3[WINDOW_SIZE] = {0};
int index_window_3 = 0;

extern int32_t channel_values[8];

extern int32_t channel_values[];
float moving_average(float new_value, float window[], int *index)
{
    // Update the window with the new value
    window[*index] = new_value;

    // Increment the index and wrap around if necessary
    *index = (*index + 1) % WINDOW_SIZE;

    // Calculate and return the average of the window
    float sum = 0;
    for (int i = 0; i < WINDOW_SIZE; i++)
    {
        sum += window[i];
    }
    return sum / WINDOW_SIZE;
}

volatile uint32_t high_time_2 = 0; // Time when signal is HIGH
volatile uint32_t low_time_2 = 0;  // Time when signal is LOW
volatile uint32_t last_time_2 = 0; // Last time the edge was detected

volatile uint32_t high_time_3 = 0; // Time when signal is HIGH
volatile uint32_t low_time_3 = 0;  // Time when signal is LOW
volatile uint32_t last_time_3 = 0; // Last time the edge was detected

int caculate(int now, int target)
{
    err = target - now;
    err_i += err;
    err_d = err - last_err;
    last_err = err;

    fix = kp * err + ki * err_i + kd * err_d;
    return fix;
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    // 读取编码器的数值
    // new_value = quadrature_encoder_get_count(pio, sm);
    // delta = new_value - old_value; //获取反应速度的相对数值
    // old_value = new_value;

    /*读取编码器测量的速度值*/
    /*进行PID运算，得到PWM输出值*/
    cooneo_timer_callback(timer, last_call_time);

    test_timer_init();

    msg.data++;
    msg_publisher_encoder.data = delta;
    sprintf(msg_publisher_encoder_String.data.data, "%d", delta);                            /////
    msg_publisher_encoder_String.data.size = strlen(msg_publisher_encoder_String.data.data); ////
    rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
    ret = rcl_publish(&publisher_encoder, &msg_publisher_encoder, NULL); // https://github.com/micro-ROS/micro-ROS-demos/blob/humble/rclc/string_publisher/main.c#L28

    uint32_t high_time_copy = high_time_2;
    uint32_t low_time_copy = low_time_2;

    // Calculate duty cycle and frequency
    uint32_t period = high_time_copy + low_time_copy;
    float duty_cycle = (float)high_time_copy / (float)period * 100.0f;
    float frequency = 1.0f / ((float)period / 1000000.0f);

    // 26,27
    //  msg_move_x.data = read_adc(0) ;
    //  msg_move_x.data = channel_values[0];
    //  ret = rcl_publish(&publisher_move_x, &msg_move_x, NULL);
    //  msg_move_y.data = read_adc(1) ;

    // msg_move_y.data = duty_cycle;
    // msg_move_y.data = duty_cycle;
    // msg_move_y.data = moving_average(duty_cycle, window_2, &index_window_2);
    // float filtered_duty_cycle_0 = moving_average(duty_cycle_0, window, &index);
    // msg_move_y.data = channel_values[2];
    // msg_move_y.data = moving_average(channel_values[2], window_2, &index_window_2);
    // ret = rcl_publish(&publisher_move_y, &msg_move_y, NULL);

    // high_time_copy = high_time_3;
    // low_time_copy = low_time_3;
    // period = high_time_copy + low_time_copy;
    // duty_cycle = (float)high_time_copy / (float)period * 100.0f;
    // frequency = 1.0f / ((float)period / 1000000.0f);
    // msg_move_x.data = moving_average(duty_cycle, window_3, &index_window_3);

    // msg_move_x.data = channel_values[0];

    /////
    /////
    /////
    /////
    msg_move_x.data = moving_average(channel_values[0], window_3, &index_window_3);
    //float _vv = mapInputToOutput(channel_values[0]);
    msg_move_x.data = map_value(msg_move_x.data, 1000, 2000, -1, 1);

    if(msg_move_x.data > 0){
        msg_move_x.data = msg_move_x.data > 0.1 ? msg_move_x.data : 0;
        msg_move_x.data = msg_move_x.data > 1 ? 1 : msg_move_x.data;
    }
    else{
        msg_move_x.data = msg_move_x.data < -0.1 ? msg_move_x.data : 0;
        msg_move_x.data = msg_move_x.data < -1 ? -1 : msg_move_x.data;
    }
    ret = rcl_publish(&publisher_move_x, &msg_move_x, NULL);
    // vv = (int)msg_move_x.data;
    // //////
    // //////
    // //////
    // //////
    msg_move_y.data = moving_average(channel_values[2], window_2, &index_window_2);
    msg_move_y.data = map_value(msg_move_y.data, 1000, 2000, -1, 1);
    
    if(msg_move_y.data>0){
        msg_move_y.data = msg_move_y.data > 0.1 ? msg_move_y.data : 0;
        msg_move_y.data = msg_move_y.data > 1 ? 1 : msg_move_y.data;
    }
    else{
        msg_move_y.data = msg_move_y.data < -0.1 ? msg_move_y.data : 0;
        msg_move_y.data = msg_move_y.data < -1 ? -1 : msg_move_y.data;
    }
    //float _ww = mapInputToOutput(channel_values[2]);
    ret = rcl_publish(&publisher_move_y, &msg_move_y, NULL);
    //ww = (int)msg_move_y.data ;


    // double _vv = map_value(vv, 900, 1400, -75, 75);
    // double _ww = map_value(ww, 900, 1400, -1, 1);

    // calculate_wheel_speeds(vv , ww, CAR_WIDTH , &vRR,&vRL);
    
    // vRR = map_value(vRR, -112.5, +112.5, -1, 1);
    // vRL = map_value(vRL, -112.5, +112.5, -1, 1);

    // //900 1400 75
    // vRR = moving_average(channel_values[2], window_2, &index_window_2);
    // vRR = map_value(vRR, 1000, 2000, 0, 1);
    // vRL = moving_average(channel_values[0], window_3, &index_window_3);
    // vRL = map_value(vRL, 900, 1400, 0, 1);



    // vRR = msg_move_x.data ;
    // vRL = msg_move_y.data ;

    
    // cooneo_inverse_2WD(msg_move_x.data,msg_move_y.data,&vRL,&vRR);

    // if(vRR > 0)
    // {
    //     motorA_forward();
    // }
    // else
    // {
    //     motorA_backward();
    //     vRR = -vRR;
    // }
    // if(vRL > 0){
    //     motorB_forward(); 
    // }
    // else{
    //     motorB_backward();
    //     vRL = -vRL; 
    // }

    // setup_pwm(PWMA, vRR);
    // setup_pwm(PWMB, vRL);

}

float speed_value, angle_value;
void subscription_callback_speed_change(const void *msgin_diy)
{
    // Cast received message to used type
    const std_msgs__msg__Int32 *msg_diy = (const std_msgs__msg__Int32 *)msgin_diy;

    // speed_value = (float)msg_diy->data / 100 ;
    speed_value = (float)msg_diy->data;
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, _value * 62500);
}

void subscription_callback_angle_change(const void *msgin_diy)
{
    // Cast received message to used type
    const std_msgs__msg__Int32 *msg_diy = (const std_msgs__msg__Int32 *)msgin_diy;

    // angle_value = (float)msg_diy->data / 100 ;
    angle_value = (float)msg_diy->data;
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, _value * 62500);
}


rcl_timer_t timer;
rcl_node_t node;
rcl_allocator_t allocator;
rclc_support_t support;
rclc_executor_t executor;

int main()
{
    // stdio_init_all();

    //set_microros_wifi_transports("OTA", "12345678", "192.168.198.125", 4444);

    // rmw_uros_set_custom_transport(
    //     true,
    //     NULL,
    //     pico_serial_transport_open,
    //     pico_serial_transport_close,
    //     pico_serial_transport_write,
    //     pico_serial_transport_read);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // adc_init();
    // adc_gpio_init(26);
    // adc_gpio_init(27);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    adc_select_input(1);

    ppm_init();

    // allocator = rcl_get_default_allocator();

    // rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    // if (ret != RCL_RET_OK)
    // {
    //     // Unreachable agent, exiting program.
    //     return ret;
    // }

    // rclc_support_init(&support, 0, NULL, &allocator);

    // rclc_node_init_default(&node, "pico_node", "", &support);
    // rclc_publisher_init_default(
    //     &publisher,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    //     "pico_publisher");

    // rclc_publisher_init_default(
    //     &publisher_move_x,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
    //     "pico_publisher_move_x");

    // rclc_publisher_init_default(
    //     &publisher_move_y,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
    //     "pico_publisher_move_y");

    // rclc_publisher_init_default(
    //     &publisher_encoder,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), /////
    //     "pico_publisher_encoder");

    // rclc_subscription_init_default(
    //     &subscriber_speed_change,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    //     "speed_change");

    // rclc_subscription_init_default(
    //     &subscriber_angle_change,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    //     "angle_change");

    // rclc_timer_init_default(
    //     &timer,
    //     &support,
    //     RCL_MS_TO_NS(2),
    //     timer_callback);

    // rclc_executor_init(&executor, &support.context, 5, &allocator);
    // rclc_executor_add_timer(&executor, &timer);

    // rclc_executor_add_subscription(&executor, &subscriber_speed_change, &msg_subscriber_speed_change, &subscription_callback_speed_change, ON_NEW_DATA);
    // rclc_executor_add_subscription(&executor, &subscriber_angle_change, &msg_subscriber_angle_change, &subscription_callback_angle_change, ON_NEW_DATA);

    //gpio_put(LED_PIN, 1);

    msg.data = 0;
    int qx;
    // int new_value, delta, old_value = 0;
    // 编码器一根线接到Pin10 另外一根接到11
    const uint PIN_AB = 10;

    PIO pio = pio0;
    const uint sm = 0;
    pio_add_program(pio, &quadrature_encoder_program);
    // quadrature_encoder_program_init(pio, sm, PIN_AB, 0);

    drv_pwm_init();
    motorA_gpio_init();
    motorB_gpio_init();
    //setup_pwm(PWMA, 0.5);

    //test_node_create();
    test_publish_init();
    test_timer_init();


    struct repeating_timer timer;
    add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer);

    ////
    while (true)
    {
        //rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

        // output_pwm += caculate(delta,30,1, 0.1, 0);

        new_value = quadrature_encoder_get_count(pio, sm);
        delta = new_value - old_value; // 获取反应速度的相对数值
        old_value = new_value;

        // output_pwm += caculate(delta, 25);

        // if (output_pwm > 65000)
        // {
        //     output_pwm = 65000;
        // }

        // if (output_pwm < 0)
        // {
        //     output_pwm = 0;
        // }

        // pwm_set_chan_level(slice_num, PWM_CHAN_A, speed_value * 62500);
        // pwm_set_chan_level(slice_num, PWM_CHAN_A, speed_value * 62500);
        // pwm_set_chan_level(slice_num_L_pwm_A, PWM_CHAN_A,speed_value * 62500);
        // pwm_set_chan_level(slice_num_L_pwm_B, PWM_CHAN_B,speed_value * 31250);
        // if (speed_value > 50)
        // {
        //     pwm_set_gpio_level(GPIO_motor_L_pwm_A, (speed_value - 50) * 2 * 625);
        //     pwm_set_gpio_level(GPIO_motor_L_pwm_B, 0);
        //     pwm_set_gpio_level(GPIO_motor_R_pwm_A, (speed_value - 50) * 2 * 625);
        //     pwm_set_gpio_level(GPIO_motor_R_pwm_B, 0);
        // }
        // else
        // {
        //     pwm_set_gpio_level(GPIO_motor_L_pwm_B, (50 - speed_value) * 2 * 625);
        //     pwm_set_gpio_level(GPIO_motor_L_pwm_A, 0);
        //     pwm_set_gpio_level(GPIO_motor_R_pwm_B, (50 - speed_value) * 2 * 625);
        //     pwm_set_gpio_level(GPIO_motor_R_pwm_A, 0);
        // }
        // // uint16_t level = (uint16_t)(( speed_value/100 * 0.5f + 0.5f) * (float)0xFFFF);
        // // pwm_set_gpio_level(GPIO_servo_pwm, level);
        // pwm_set_gpio_level(GPIO_servo_pwm, (angle_value) * 3125 + 3125); // TEST:ok 50HZ

        // pwm_set_chan_level(slice_num, PWM_CHAN_A, output_pwm);
        // motor_speed_set(1,2);

        // pwm_set_gpio_level(MOTOR_PIN_1, 31250); // 1.5ms pulse width
        // pwm_set_gpio_level(MOTOR_PIN_2, 15625); // 1.5ms pulse width
        // motorA_forward();
        // sleep_ms(1000);
        // motorA_backward();
        // sleep_ms(1000);
        // motorA_stop();
        // sleep_ms(1000);
        // sleep_ms(20);
    }
    return 0;
}

