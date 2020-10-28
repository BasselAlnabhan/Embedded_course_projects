#ifndef BSP_H
#define BSP_H

#include <stdint.h>

#define LOW 0
#define HIGH 1
#define OUTPUT 1

void bsp_digital_write(uint8_t pin, uint8_t val);
void bsp_pin_mode(uint8_t pin, uint8_t mode);
void bsp_delay(uint32_t msec);

#endif /* BSP_H */