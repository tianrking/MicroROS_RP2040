#include <stdio.h>

#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "quadrature_encoder.pio.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/string.h>
#include <rmw_microros/rmw_microros.h>

#include "pico/stdlib.h"
#include "pico_uart_transports.h"


int wrap;
int GPIO_motor_pwm = 6;
int new_value, delta, old_value = 0;
const uint sm = 0;
PIO pio = pio0;

const uint LED_PIN = 25;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rcl_publisher_t publisher_encoder;
std_msgs__msg__Int32 msg_publisher_encoder;
std_msgs__msg__String msg_publisher_encoder_String;

rcl_subscription_t subscriber_speed_change;
std_msgs__msg__Int32 msg_subscriber_speed_change;

float output_pwm = 0; // 12000
float kp=5, ki=0.4, kd=0;
float err, last_err;
float err_i;
float err_d;
float fix;

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
    
    msg.data++;
    msg_publisher_encoder.data = delta ;

    sprintf(msg_publisher_encoder_String.data.data, "%d", delta); /////
    msg_publisher_encoder_String.data.size = strlen(msg_publisher_encoder_String.data.data); ////
    rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
    ret = rcl_publish(&publisher_encoder, &msg_publisher_encoder, NULL); // https://github.com/micro-ROS/micro-ROS-demos/blob/humble/rclc/string_publisher/main.c#L28

}

float speed_value;
void subscription_callback_speed_change(const void *msgin_diy)
{
    // Cast received message to used type
    const std_msgs__msg__Int32 *msg_diy = (const std_msgs__msg__Int32 *)msgin_diy;

    speed_value = (float)msg_diy->data / 100 ;
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, _value * 62500);

}

int main()
{
   
    rmw_uros_set_custom_transport(
		true,
		NULL,
		pico_serial_transport_open,
		pico_serial_transport_close,
		pico_serial_transport_write,
		pico_serial_transport_read
	);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rcl_timer_t timer;
    rcl_node_t node;
    rcl_allocator_t allocator;
    rclc_support_t support;
    rclc_executor_t executor;

    allocator = rcl_get_default_allocator();

    // Wait for agent successful ping for 2 minutes.
    const int timeout_ms = 1000; 
    const uint8_t attempts = 120;

    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    if (ret != RCL_RET_OK)
    {
        // Unreachable agent, exiting program.
        return ret;
    }

    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, "pico_node", "", &support);
    rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_publisher");

    rclc_publisher_init_default(
        &publisher_encoder,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), /////
        "pico_publisher_encoder");

    rclc_subscription_init_default(
        &subscriber_speed_change,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "speed_change");

    rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(2),
        timer_callback);

    rclc_executor_init(&executor, &support.context, 5, &allocator);
    rclc_executor_add_timer(&executor, &timer);

    rclc_executor_add_subscription(&executor, &subscriber_speed_change, &msg_subscriber_speed_change, &subscription_callback_speed_change, ON_NEW_DATA);


    gpio_put(LED_PIN, 1);

    msg.data = 0;

    // int new_value, delta, old_value = 0;
    //编码器一根线接到Pin10 另外一根接到11
    const uint PIN_AB = 10;

    // PIO pio = pio0;
    // const uint sm = 0;
    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, offset, PIN_AB, 0);

    // 选择输出 pwm 的引脚 用作控制信号传入L298n 电机驱动并完成初始化
    gpio_set_function(GPIO_motor_pwm, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(GPIO_motor_pwm);
    wrap = 62500; // 2khz
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);

    // 在这里我们设定默认输出 占空比为0 即 静止状态
    // float output_pwm = 0; // 12000

    while (true)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

        // output_pwm += caculate(delta,30,1, 0.1, 0);
        
        new_value = quadrature_encoder_get_count(pio, sm);
        delta = new_value - old_value; //获取反应速度的相对数值
        old_value = new_value;

        output_pwm += caculate(delta, 25);

        if (output_pwm > 65000)
        {
            output_pwm = 65000;
        }

        if (output_pwm < 0)
        {
            output_pwm = 0;
        }

        pwm_set_chan_level(slice_num, PWM_CHAN_A, speed_value * 62500);
        // pwm_set_chan_level(slice_num, PWM_CHAN_A, output_pwm);
        
        sleep_ms(20);

    }
    return 0;
}