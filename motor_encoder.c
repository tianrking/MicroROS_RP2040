#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

// 定义编码器引脚
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

// 旋转编码器每一圈的计数
#define COUNTS_PER_REV 500

// 速度计算周期（毫秒）
#define SPEED_CALC_INTERVAL 100

volatile int32_t current_count = 0;

void encoder_interrupt_handler(uint gpio, uint32_t events) {
    static uint8_t encoder_state = 0;

    // 获取编码器A和B引脚的状态
    encoder_state = (encoder_state << 2) | ((gpio_get(ENCODER_B_PIN) << 1) | gpio_get(ENCODER_A_PIN));
    encoder_state &= 0x0F;

    // 根据状态确定方向并增加或减少计数值
    if (encoder_state == 0x01 || encoder_state == 0x0E) {
        current_count++;
    } else if (encoder_state == 0x04 || encoder_state == 0x0B) {
        current_count--;
    }
}

// 速度计算定时器回调
void speed_calculation_callback() {
    static int32_t last_count = 0;

    int32_t count_diff = current_count - last_count;
    last_count = current_count;

    float speed_rpm = (float)count_diff / COUNTS_PER_REV * 60000.0f / SPEED_CALC_INTERVAL;

    printf("Speed: %.2f RPM\n", speed_rpm);
}

int main() {
    stdio_init_all();

    // 初始化编码器输入引脚
    gpio_init(ENCODER_A_PIN);
    gpio_init(ENCODER_B_PIN);
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_dir(ENCODER_B_PIN, GPIO_IN);

    // 设置编码器引脚中断处理程序
    gpio_set_irq_enabled_with_callback(ENCODER_A_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_interrupt_handler);
    gpio_set_irq_enabled_with_callback(ENCODER_B_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_interrupt_handler);

    // 添加定时器回调以计算速度
    add_repeating_timer_ms(SPEED_CALC_INTERVAL, speed_calculation_callback, NULL, NULL);

    while (1) {
        sleep_ms(10);
    }

    return 0;
}

