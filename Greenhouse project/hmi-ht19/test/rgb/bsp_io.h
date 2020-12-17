#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

void bsp_pin_mode(uint8_t pin, uint8_t mode);
void bsp_digital_write(uint8_t pin, uint8_t val);

#endif