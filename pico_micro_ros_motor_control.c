#include "pico_micro_ros_motor_control.h"

#define REMOTE_MAX 2000
#define REMOTE_MIN 1000
#define REMOTE_MID 1500

int wrap;
int GPIO_motor_L_pwm_A = 6;
int GPIO_motor_L_pwm_B = 7;
int GPIO_motor_R_pwm_A = 8;
int GPIO_motor_R_pwm_B = 9;
int GPIO_servo_pwm = 5;

int new_value, delta, old_value = 0;
const uint sm = 0;
PIO pio = pio0;

const uint LED_PIN = 25;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rcl_publisher_t publisher_move_x;
std_msgs__msg__Float64 msg_move_x;
rcl_publisher_t publisher_move_y;
std_msgs__msg__Float64 msg_move_y;
rcl_publisher_t publisher_move_switch;
std_msgs__msg__Float64 msg_move_switch;

rcl_publisher_t publisher_encoder;
std_msgs__msg__Int32 msg_publisher_encoder;
std_msgs__msg__String msg_publisher_encoder_String;

rcl_subscription_t subscriber_speed_change;
std_msgs__msg__Int32 msg_subscriber_speed_change;

rcl_subscription_t subscriber_angle_change;
std_msgs__msg__Int32 msg_subscriber_angle_change;

rcl_subscription_t subscriber_twist;
geometry_msgs__msg__Twist msg_twit;

rcl_publisher_t publisher_encoder_move_x;
std_msgs__msg__Int32 msg_publisher_move_x;
rcl_publisher_t publisher_encoder_move_y;
std_msgs__msg__Int32 msg_publisher_move_y;

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
float window_5[WINDOW_SIZE] = {0};
int index_window_5 = 0;
int twist_flag_temp;
int twist_flag;

#define PPM_PIN 22

uint64_t last_time = 0;
uint32_t channel_values[8];

// Pin definitions COONEO MOTOR
const uint AIN1 = 3;
const uint AIN2 = 2;
const uint PWMA = 12;

const uint BIN2 = 10; // switch BIN2 BIN1 mirror install
const uint BIN1 = 11;
const uint PWMB = 13;

const uint SERVO_PIN = 26; // COONEO SERVO3

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

// #define PPM_PIN_2 2 // The GPIO pin number where PPM signal is connected
// #define PWM_PIN_2 2 // GPIO pin where PWM signal is connected
// #define PPM_PIN_3 3 // The GPIO pin number where PPM signal is connected
// #define PWM_PIN_3 3

#define PPM_PIN_22 22 // 29

volatile uint32_t high_time_2 = 0; // Time when signal is HIGH
volatile uint32_t low_time_2 = 0;  // Time when signal is LOW
volatile uint32_t last_time_2 = 0; // Last time the edge was detected

volatile uint32_t high_time_3 = 0; // Time when signal is HIGH
volatile uint32_t low_time_3 = 0;  // Time when signal is LOW
volatile uint32_t last_time_3 = 0; // Last time the edge was detected

// void gpio_2_callback(uint gpio, uint32_t events)
// {
//     uint32_t time_now = time_us_32();
//     uint32_t duration = time_now - last_time_2;
//     last_time_2 = time_now;

//     if (gpio_get(PPM_PIN_22))
//     {
//         // Rising edge detected
//         low_time_2 = duration;
//     }
//     else
//     {
//         // Falling edge detected
//         high_time_2 = duration;
//     }
// }
// void gpio_3_callback(uint gpio, uint32_t events)
// {
//     uint32_t time_now = time_us_32();
//     uint32_t duration = time_now - last_time_3;
//     last_time_3 = time_now;

//     if (gpio_get(PWM_PIN_3))
//     {
//         // Rising edge detected
//         low_time_3 = duration;
//     }
//     else
//     {
//         // Falling edge detected
//         high_time_3 = duration;
//     }
// }

// volatile uint32_t channel_values[8]; // To store decoded channel values
// volatile uint32_t last_time;
// volatile uint8_t channel;

////////////////////////////////// test block
// #define PPM_PIN 22

// uint64_t last_time = 0;
// uint32_t channel_values[8];

void ppm_callback(uint gpio, uint32_t events)
{
    uint64_t now = time_us_64();
    uint32_t duration = (uint32_t)(now - last_time);
    last_time = now;

    static int channel = 0;
    static int sync = 0;

    if (duration > 4000)
    {
        // Sync pulse detected
        sync = 1;
        channel = 0;
    }
    else if (sync)
    {
        // Record the pulse length for each channel
        if (channel < 8)
        {
            channel_values[channel] = duration;
        }
        channel++;
    }
}

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

int caculate(int now, int target)
{
    err = target - now;
    err_i += err;
    err_d = err - last_err;
    last_err = err;

    fix = kp * err + ki * err_i + kd * err_d;
    return fix;
}

uint16_t read_adc(uint channel)
{
    adc_select_input(channel);
    return adc_read();
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

    msg_move_switch.data = moving_average(channel_values[5], window_5, &index_window_5);
    msg_move_switch.data = msg_move_switch.data > 1400 ? 1:0;

    if((int)msg_move_switch.data == 1){
        twist_flag_temp =1 ;
    }
    else  if((int)msg_move_switch.data == 0)
    {
        if(twist_flag_temp == 1)
        {
            twist_flag ++;
            twist_flag_temp = 0;
        }

    }
    else{
        ;
    }

    msg_move_switch.data = twist_flag % 2; // 1 for nav2 0 for remote
    ret = rcl_publish(&publisher_move_switch, &msg_move_switch, NULL);
    
    msg_move_y.data = moving_average(channel_values[2], window_2, &index_window_2);
    ret = rcl_publish(&publisher_move_y, &msg_move_y, NULL);

    float _speed_temp = mapInputToOutput(channel_values[2], T_MOTOR);  //for v
    if (_speed_temp > 0 && (twist_flag % 2)==0)
    {
        if((twist_flag % 2))
            ;
        else{
            motor_control(FORWARD);
            motor_set_speed(PWMA, _speed_temp);
            motor_set_speed(PWMB, _speed_temp);
        }
    }
    else
    {
        if((twist_flag % 2))
            ;
        else{
            motor_control(BACKWARD);
            motor_set_speed(PWMA, -_speed_temp);
            motor_set_speed(PWMB, -_speed_temp);
        }
    }

    // high_time_copy = high_time_3;
    // low_time_copy = low_time_3;
    // period = high_time_copy + low_time_copy;
    // duty_cycle = (float)high_time_copy / (float)period * 100.0f;
    // frequency = 1.0f / ((float)period / 1000000.0f);
    // msg_move_x.data = moving_average(duty_cycle, window_3, &index_window_3);

    // msg_move_x.data = channel_values[0];
    msg_move_x.data = moving_average(channel_values[0], window_3, &index_window_3);  /// for w
    msg_move_x.data = mapInputToOutput(msg_move_x.data, T_SERVO);   /// for w

    if((twist_flag % 2)==0){
        servo_set_angle((int)SERVO_PIN, msg_move_x.data);
    }
    else{
        ; //nav
    }


    ret = rcl_publish(&publisher_move_x, &msg_move_x, NULL);
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

float linear_velocity ;
float angular_velocity ;
void subscription_twist_callback(const void * msgin) {
    const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;

    // Extract linear and angular velocities
    linear_velocity = msg->linear.x;
    angular_velocity = msg->angular.z;

    // Control the car using the extracted velocities
    // This is just a placeholder, replace with your car's driver function
    // car_move(linear_velocity, angular_velocity);
}


/////////////////////////////cooneo motor control

void motor_set_speed(uint gpio, float duty_cycle)
{
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 255);
    pwm_set_gpio_level(gpio, duty_cycle * 255);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    pwm_set_enabled(slice_num, true);
}

void motor_control(MOTOR_STATE _state)
{
    switch (_state)
    {
    case FORWARD:
        gpio_put(AIN1, 1);
        gpio_put(AIN2, 0);
        gpio_put(BIN1, 1);
        gpio_put(BIN2, 0);
        break;

    case BACKWARD:
        // 處理向後既情況
        gpio_put(AIN1, 0);
        gpio_put(AIN2, 1);
        gpio_put(BIN1, 0);
        gpio_put(BIN2, 1);
        break;

    case STOP:
        // 處理停止既情況
        gpio_put(AIN1, 0);
        gpio_put(AIN2, 0);
        gpio_put(BIN1, 0);
        gpio_put(BIN2, 0);
        break;

    default:
        // 預設情況
        gpio_put(AIN1, 0);
        gpio_put(AIN2, 0);
        gpio_put(BIN1, 0);
        gpio_put(BIN2, 0);
        break;
    }
}

void motor_gpio_init()
{
    gpio_init(AIN1);
    gpio_init(AIN2);
    gpio_set_dir(AIN1, GPIO_OUT);
    gpio_set_dir(AIN2, GPIO_OUT);
    gpio_init(BIN1);
    gpio_init(BIN2);
    gpio_set_dir(BIN1, GPIO_OUT);
    gpio_set_dir(BIN2, GPIO_OUT);
}

//////////// cooneo servo control

#define DUTY_MIN 2400
#define DUTY_MAX 8000
#define TOP_MAX 65534

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_init(uint gpio,int base_frequency)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);


    uint32_t source_hz = clock_get_hz(clk_sys);
        uint freq = 50;
        uint32_t div16_top = 16 * source_hz / freq;
        uint32_t top = 1;
        for (;;) {
            if (div16_top >= 16 * 5 && div16_top % 5 == 0 && top * 5 <= TOP_MAX) {
                div16_top /= 5;
                top *= 5;
            } else if (div16_top >= 16 * 3 && div16_top % 3 == 0 && top * 3 <= TOP_MAX) {
                div16_top /= 3;
                top *= 3;
            } else if (div16_top >= 16 * 2 && top * 2 <= TOP_MAX) {
                div16_top /= 2;
                top *= 2;
            } else {
                break;
            }
        }
        pwm_hw->slice[slice].div = div16_top;
        pwm_hw->slice[slice].top = top;
}

// Function to set servo angle
void servo_set_angle(uint gpio, int angle)
{
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    uint32_t top = pwm_hw->slice[slice].top;
    int duty_u16 = map(angle, 0, 180, DUTY_MIN, DUTY_MAX);
    uint32_t cc = duty_u16 * (top + 1) / 65535;
    pwm_set_chan_level(slice, channel, cc);
    pwm_set_enabled(slice, true);
}

////////////////////////////////////
#define _RM_min 900
#define _RM_max 1900
#define _RM_mid 1400
#define _RM_constrain_min 1000
#define _RM_constrain_max 1800

int constrainInput(int input)
{
    if (input < _RM_constrain_min )
        return _RM_constrain_min;
    if (input > _RM_constrain_max)
        return _RM_constrain_max;
    return input;
}

float mapInputToOutput(int input, CONTROL_TARGET _ct)
{
    float output;
    input = constrainInput(input); // 使用封裝好的函數來限制輸入

    switch (_ct)
    {
    case T_MOTOR:
        if (input <= _RM_mid)
        {
            output = (float)(input - _RM_mid) / (_RM_mid - _RM_min);
        }
        else
        {
            output = (float)(input - _RM_mid) / (_RM_max - _RM_mid);
        }
        break;

    case T_SERVO:
        // if (input <= _RM_mid)
        // {
        //     output = (float)(input - _RM_mid) / (_RM_mid - 900) * 45;
        // }
        // else
        // {
        //     output = (float)(input - _RM_mid) / (1900 - _RM_mid) * 45;
        // }
        // 進行線性轉換
        if (input <= _RM_mid)
        {
            output = (float)(input - _RM_mid) / (_RM_mid - _RM_min) * 50 + 50;
        }
        else
        {
            output = (float)(input - _RM_mid) / (_RM_max - _RM_mid) * 50 + 50;
        }

        break;

    default:
        // None 或者其他未定義的 CONTROL_TARGET
        if (input <= _RM_mid)
        {
            output = (float)(input - _RM_mid) / (_RM_mid - _RM_min);
        }
        else
        {
            output = (float)(input - _RM_mid) / (_RM_min - _RM_mid);
        }
        break;
    }

    return output;
}

// int main() {
//     stdio_init_all();

//     // Initialize GPIO
//     gpio_init(AIN1);
//     gpio_init(AIN2);
//     gpio_init(STBY);

//     gpio_set_dir(AIN1, GPIO_OUT);
//     gpio_set_dir(AIN2, GPIO_OUT);
//     gpio_set_dir(STBY, GPIO_OUT);

//     gpio_put(STBY, 1); // Take out of standby

//     // Setup PWM
//     setup_pwm(PWMA, 0.5); // 50% duty cycle

//     while (true) {
//         motorA_forward();
//         sleep_ms(1000);
//         motorA_backward();
//         sleep_ms(1000);
//         motorA_stop();
//         sleep_ms(1000);
//     }
// }
//////

int main()
{
    // stdio_init_all();
    rmw_uros_set_custom_transport(
        true,
        NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    adc_select_input(1);

    // PPM
    // gpio_set_irq_enabled_with_callback(PPM_PIN_3, GPIO_IRQ_EDGE_RISE, true, &ppm_callback);
    // last_time = time_us_32();
    // PWM

    // gpio_set_irq_enabled_with_callback(PPM_PIN_22, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_2_callback); //29
    // gpio_set_irq_enabled_with_callback(PWM_PIN_3, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_3_callback);

    gpio_set_dir(PPM_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(PPM_PIN, GPIO_IRQ_EDGE_RISE, true, &ppm_callback);

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
        &publisher_move_x,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
        "pico_publisher_move_x");

    rclc_publisher_init_default(
        &publisher_move_y,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
        "pico_publisher_move_y");

    rclc_publisher_init_default(
        &publisher_move_switch,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
        "pico_publisher_move_switch");

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

    rclc_subscription_init_default(
        &subscriber_angle_change,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "angle_change");
    
    ///    
    rclc_subscription_init_default(
        &subscriber_twist,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        "/cmd_vel");
        
    // rclc_subscription_init_default(
    //     &subscriber_speed_change,
    //     &node,
    //     ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    //     "speed_change");
    ///    
        
        

    rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(2),
        timer_callback);

    rclc_executor_init(&executor, &support.context, 5, &allocator);
    rclc_executor_add_timer(&executor, &timer);

    rclc_executor_add_subscription(&executor, &subscriber_speed_change, &msg_subscriber_speed_change, &subscription_callback_speed_change, ON_NEW_DATA);
    rclc_executor_add_subscription(&executor, &subscriber_angle_change, &msg_subscriber_angle_change, &subscription_callback_angle_change, ON_NEW_DATA);
    rclc_executor_add_subscription(&executor, &subscriber_twist, &msg_twit, &subscription_twist_callback, ON_NEW_DATA);

        //rclc_wait_set_add_subscription(&wait_set, &subscriber_twist, NULL);
    gpio_put(LED_PIN, 1);

    msg.data = 0;
    int qx;
    // int new_value, delta, old_value = 0;
    // 编码器一根线接到Pin10 另外一根接到11
    const uint PIN_AB = 10;

    PIO pio = pio0;
    const uint sm = 0;
    pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, sm, PIN_AB, 0);

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

    // 在这里我们设定默认输出 占空比为0 即 静止状态
    // float output_pwm = 0; // 12000

    // gpio_motor_init();

    // const uint PWM_FREQUENCY = 50; // 50 Hz

    // // Initialize the PWM for the first pin
    // gpio_set_function(MOTOR_PIN_1, GPIO_FUNC_PWM);
    // uint slice1 = pwm_gpio_to_slice_num(MOTOR_PIN_1);
    // // pwm_set_wrap(slice1, 19999);  // for 50Hz with a system clock of 2MHz
    // // pwm_init(slice1, &config1, true);
    // wrap = 62500; // 2khz
    // pwm_set_wrap(slice_num_servo_pwm, wrap);
    // pwm_set_clkdiv(MOTOR_PIN_1, 40.0f);
    // pwm_set_enabled(MOTOR_PIN_1, true);

    // // Initialize the PWM for the second pin
    // gpio_set_function(MOTOR_PIN_2, GPIO_FUNC_PWM);
    // uint slice2 = pwm_gpio_to_slice_num(MOTOR_PIN_2);
    // // pwm_set_wrap(slice2, 19999);  // for 50Hz with a system clock of 2MHz
    // // pwm_init(slice2, &config2, true);
    // wrap = 62500; // 2khz
    // pwm_set_wrap(slice_num_servo_pwm, wrap);
    // pwm_set_clkdiv(MOTOR_PIN_2, 40.0f);
    // pwm_set_enabled(MOTOR_PIN_2, true);

    //////test
    motor_gpio_init();

    servo_init(SERVO_PIN, 50);
    //servo_init(SERVO_PIN);
    
    // setup_pwm(PWMA, 0.5);

    ////
    while (true)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

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
        if (speed_value > 50)
        {
            pwm_set_gpio_level(GPIO_motor_L_pwm_A, (speed_value - 50) * 2 * 625);
            pwm_set_gpio_level(GPIO_motor_L_pwm_B, 0);
            pwm_set_gpio_level(GPIO_motor_R_pwm_A, (speed_value - 50) * 2 * 625);
            pwm_set_gpio_level(GPIO_motor_R_pwm_B, 0);
        }
        else
        {
            pwm_set_gpio_level(GPIO_motor_L_pwm_B, (50 - speed_value) * 2 * 625);
            pwm_set_gpio_level(GPIO_motor_L_pwm_A, 0);
            pwm_set_gpio_level(GPIO_motor_R_pwm_B, (50 - speed_value) * 2 * 625);
            pwm_set_gpio_level(GPIO_motor_R_pwm_A, 0);
        }
        // uint16_t level = (uint16_t)(( speed_value/100 * 0.5f + 0.5f) * (float)0xFFFF);
        // pwm_set_gpio_level(GPIO_servo_pwm, level);
        pwm_set_gpio_level(GPIO_servo_pwm, (angle_value) * 3125 + 3125); // TEST:ok 50HZ

        // pwm_set_chan_level(slice_num, PWM_CHAN_A, output_pwm);
        // motor_speed_set(1,2);

        // pwm_set_gpio_level(MOTOR_PIN_1, 31250); // 1.5ms pulse width
        // pwm_set_gpio_level(MOTOR_PIN_2, 15625); // 1.5ms pulse width

        // motor_control(FORWARD);
        // sleep_ms(1000);
        // motor_control(STOP);
        // sleep_ms(1000);
        // motor_control(BACKWARD);
        // sleep_ms(1000);

        //rcl_wait_set_t wait_set = rcl_get_zero_initialized_wait_set();
        //rcl_wait_set_t wait_set = rcl_get_zero_initialized_wait_set();
        // rclc_wait_set_init(&wait_set, 1, 0, 0, 0, 0, 0, &support.context, &allocator);

        // // // Add our subscription to the wait set
        // rclc_wait_set_add_subscription(&wait_set, &subscriber_twist, NULL);



        sleep_ms(20);
    }

    // Clean up (This part of the code will never be reached in this example)
    //rclc_subscription_fini(&subscriber, &node);
    //rclc_node_fini(&node);
    return 0;
}
