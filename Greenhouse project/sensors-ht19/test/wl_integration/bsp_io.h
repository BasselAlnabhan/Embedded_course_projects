#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

void bsp_analog_read_resolution(uint8_t bits);
uint16_t bsp_analog_read(uint8_t pin);

#endif