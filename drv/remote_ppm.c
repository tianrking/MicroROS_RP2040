#include "main.h"

uint64_t last_time = 0;
int32_t channel_values[8];

void ppm_init(void)
{
    gpio_set_dir(PPM_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(PPM_PIN, GPIO_IRQ_EDGE_RISE, true, &ppm_callback);
}

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