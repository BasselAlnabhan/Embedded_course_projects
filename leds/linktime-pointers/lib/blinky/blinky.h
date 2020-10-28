#ifndef BLINKY_H
#define BLINKY_H

#include <stdint.h>

#define MIN_LEDS (1U)
#define MAX_LEDS (32U)

#define MIN_BLINK_DELAY (10U)
#define MAX_BLINK_DELAY (1000U)

#define BLINKY_ERROR (0U)
#define BLINKY_SUCCESS (1U)

typedef struct
{
    void (*digital_write)(uint8_t, uint8_t);
    void (*pin_mode)(uint8_t, uint8_t);
    void (*delay_msec)(uint32_t);
} interface_t;

/**
 * @brief This function is used to initialize the module
 * 
 * @param intrfc An interface pointer. If this parameter is NULL, the default functions will be used.
 * @param pins A pointer which points to the array of the pin numbers connected to the leds
 * @param number_of_leds Number of the leds
 * @return uint8_t If there is an error BLINKY_ERROR is returned. Otherwise BLINKY_SUCCESS
 */
uint8_t blinky_begin(interface_t *intrfc, uint8_t *pins, uint8_t number_of_leds);

/**
 * @brief This function is used to make the leds blink N times with the provided value of the used delay.
 * 
 * @param times Number of the blinks
 * @param delay_ms The delay used for make the leds blink
 * @return uint8_t If there is an error BLINKY_ERROR is returned. Otherwise BLINKY_SUCCESS
 */
uint8_t blinky_blink_times(uint8_t times, uint16_t delay_ms);

/**
 * @brief This function is used to reset the module to the default state.
 * 
 */
void blinky_end(void);

#endif /* BLINKY_H */