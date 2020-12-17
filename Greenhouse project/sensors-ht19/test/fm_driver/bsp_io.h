#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif

#ifndef RISING
#define RISING 3
#endif

void bsp_pin_mode(uint8_t pin, uint8_t mode);
void bsp_attach_interrupt(uint8_t pin, void (*function)(void), int mode);

void bsp_interrupts(void);
void bsp_no_interrupts(void);
#endif