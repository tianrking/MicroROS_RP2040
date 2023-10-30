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