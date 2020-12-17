#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

uint16_t bsp_analog_read(uint8_t pin);
void bsp_analog_read_resolution(uint8_t bits);

#endif