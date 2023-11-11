#ifndef DRV_REMOTE_PPM_H
#define DRV_REMOTE_PPM_H

#define PPM_PIN 22

void ppm_init(void);
void ppm_callback(uint gpio, uint32_t events);

#endif