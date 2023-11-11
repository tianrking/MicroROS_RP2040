#include "main.h"

uint16_t read_adc(uint channel)
{
    adc_select_input(channel);
    return adc_read();
}