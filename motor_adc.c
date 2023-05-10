#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// 定义ADC引脚
#define ADC_PIN_1 26
#define ADC_PIN_2 27
#define ADC_PIN_3 28
#define ADC_PIN_4 29

// ADC检测周期（毫秒）
#define ADC_READ_INTERVAL 50

void init_adc(uint gpio) {
    adc_gpio_init(gpio);
    adc_select_input(gpio - 26);
}

uint16_t read_adc(uint gpio) {
    adc_select_input(gpio - 26);
    return adc_read();
}

int main() {
    stdio_init_all();

    // 初始化ADC
    adc_init();

    // 设置ADC时钟
    adc_set_clkdiv(24000000 / 1000000);

    // 初始化ADC引脚
    init_adc(ADC_PIN_1);
    init_adc(ADC_PIN_2);
    init_adc(ADC_PIN_3);
    init_adc(ADC_PIN_4);

    uint16_t adc_value[4];

    while (1) {
        // 读取ADC值
        adc_value[0] = read_adc(ADC_PIN_1);
        adc_value[1] = read_adc(ADC_PIN_2);
        adc_value[2] = read_adc(ADC_PIN_3);
        adc_value[3] = read_adc(ADC_PIN_4);

        // 打印ADC值
        printf("ADC Values: %d, %d, %d, %d\n", adc_value[0], adc_value[1], adc_value[2], adc_value[3]);

        // 等待下一次读取
        sleep_ms(ADC_READ_INTERVAL);
    }

    return 0;
}

