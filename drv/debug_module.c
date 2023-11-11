#include "main.h"

rcl_timer_t cooneo_timer;
rcl_node_t cooneo_node;
rcl_allocator_t cooneo_allocator;
rclc_support_t cooneo_support;
rclc_executor_t cooneo_executor;

rcl_publisher_t cooneo_publisher;
std_msgs__msg__Int32 cooneo_publisher_msg;

rcl_subscription_t cooneo_subscriber;
std_msgs__msg__Int32 cooneo_subscriber_msg;

extern rcl_timer_t timer;
extern rcl_node_t node;
extern rcl_allocator_t allocator;
extern rclc_support_t support;
extern rclc_executor_t executor;


void test_node_create(void)
{
    cooneo_allocator = rcl_get_default_allocator();
    rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, attempts);

    // if (ret != RCL_RET_OK)
    // {
    //     // Unreachable agent, exiting program.
    //     return ret;
    // }

    rclc_support_init(&cooneo_support, 0, NULL, &cooneo_allocator);

    rclc_node_init_default(&cooneo_node, "cooneo_node", "", &cooneo_support);

    rclc_executor_init(&cooneo_executor, &cooneo_support.context, 5, &cooneo_allocator);
}

void test_publish_init(void)
{
    rclc_publisher_init_default(
        &cooneo_publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "cooneo_publisher");
}

void test_timer_init(void){
    rclc_executor_add_timer(&executor, &timer);
}

void test_subscribe_init(void)
{
    rclc_subscription_init_default(
        &cooneo_subscriber,
        &cooneo_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "cooneo_subscriber");
}

void cooneo_timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    cooneo_publisher_msg.data++;
    rcl_ret_t ret = rcl_publish(&cooneo_publisher, &cooneo_publisher_msg, NULL);

    // if (ret != RCL_RET_OK)
    // {
    //     // Unreachable agent, exiting program.
    //     return ret;
    // }
}

extern int32_t channel_values[8];
static double _v,_w,_vRL,_vRR;

#define WINDOW_SIZE 20
static float window_8[WINDOW_SIZE] = {0};
static int index_window_8 = 0;
static float window_9[WINDOW_SIZE] = {0};
static int index_window_9 = 0;

bool repeating_timer_callback(struct repeating_timer *t) {
    //printf("Repeat at %lld\n", time_us_64());
    _w= moving_average(channel_values[0], window_8, &index_window_8);
    //float _wv = mapInputToOutput(channel_walues[0]);
    _w = map_value(_w, 1000, 2000, -0.6, 0.6);

    if(_w > 0){
        _w = _w > 0.1 ? _w : 0;
        _w = _w > 1 ? 1 : _w;
    }
    else{
        _w = _w < -0.1 ? _w : 0;
        _w = _w < -1 ? -1 : _w;
    }

    _v = moving_average(channel_values[2], window_9, &index_window_9);
    _v = map_value(_v, 1000, 2000, -1, 1);
    
    if(_v>0){
        _v = _v > 0.1 ? _v : 0;
        _v = _v > 1 ? 1 : _v;
    }
    else{
        _v = _v < -0.1 ? _v : 0;
        _v = _v < -1 ? -1 : _v;
    }
    
    cooneo_inverse_2WD(_v,_w,&_vRL,&_vRR);

    if(_vRR > 0)
    {
        //motorA_forward();
        motorA_backward();
    }
    else
    {
        // motorA_backward();
        motorA_forward();
        _vRR = -_vRR;
    }
    if(_vRL > 0){
        // motorB_forward(); 
        motorB_backward();
    }
    else{
        // motorB_backward();
        motorB_forward(); 
        _vRL = -_vRL; 
    }

    if(_v = 0 && _w ==0){
        motorA_stop();
        motorB_stop();
    }

    _vRR = _vRR > 0.95 ? 0.95:_vRR;
    _vRL = _vRL > 0.95 ? 0.95:_vRL; 
    setup_pwm(PWMA, _vRR);
    setup_pwm(PWMB, _vRL);
    return true;
}