#ifndef DRV_DEBUG_MODULE_H
#define DRV_DEBUG_MODULE_H

void test_node_create(void);
void test_publish_init(void);
void test_subscribe_init(void);
void test_timer_init(void);
void cooneo_timer_callback(rcl_timer_t *timer, int64_t last_call_time);


#endif