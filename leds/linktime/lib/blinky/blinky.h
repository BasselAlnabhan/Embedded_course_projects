#ifndef BLINKY_H
#define BLINKY_H

#include <stdint.h>

#define MIN_LEDS 1
#define MAX_LEDS 32

#define MIN_BLINK_DELAY 10
#define MAX_BLINK_DELAY 1000

#define BLINKY_ERROR 0
#define BLINKY_SUCCESS 1

uint8_t blinky_begin(uint8_t *pins, uint8_t number_of_leds);
uint8_t blinky_blink_times(uint8_t times, uint16_t delay_ms);
void blinky_end(void);

#endif /* BLINKY_H */