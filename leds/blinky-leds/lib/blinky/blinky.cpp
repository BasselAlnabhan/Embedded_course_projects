#include <blinky.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

/**
 * @brief This function is used to check if the pins are unique or not
 * 
 * @param pins A pointer to the array of the pins connected to the leds
 * @param number_of_leds Number of the leds
 * @return true The pins are unique
 * @return false There is at least a duplicate pin
 */
static bool is_unique(uint8_t *pins, uint8_t number_of_leds);

static void (*digital_write)(uint8_t, uint8_t); // A function pointer for digital write function
static void (*pin_mode)(uint8_t, uint8_t);      // A function pointer for pin mode function
static void (*delay_msec)(uint32_t);            // A function pointer to make a delay
static uint8_t leds[MAX_LEDS];                  // An array of the pins connected to the leds
static uint8_t number;                          // Number of the leds

uint8_t blinky_begin(interface_t *intrfc, uint8_t *pins, uint8_t number_of_leds)
{
    blinky_end(); // Reset the state of module

    uint8_t status = BLINKY_ERROR;

    if ((number_of_leds >= MIN_LEDS) && (number_of_leds <= MAX_LEDS) && (pins != NULL))
    {
        if (is_unique(pins, number_of_leds))
        {
            if (intrfc != NULL)
            {
                digital_write = intrfc->digital_write;
                delay_msec = intrfc->delay_msec;
                pin_mode = intrfc->pin_mode;
            }

            number = number_of_leds;

            for (uint8_t i = 0; i < number; i++)
            {
                leds[i] = pins[i];
                pin_mode(pins[i], OUTPUT);
                digital_write(pins[i], LOW);
            }

            status = BLINKY_SUCCESS;
        }
    }

    return status;
}

uint8_t blinky_blink_times(uint8_t times, uint16_t delay_ms)
{
    uint8_t status = BLINKY_ERROR;

    if ((times > 0) && (number > 0))
    {
        if ((delay_ms >= MIN_BLINK_DELAY) && (delay_ms <= MAX_BLINK_DELAY))
        {
            for (uint8_t n = 0; n < times; n++)
            {
                for (uint8_t i = 0; i < number; i++)
                {
                    digital_write(leds[i], HIGH);
                }

                delay_msec(delay_ms);

                for (uint8_t i = 0; i < number; i++)
                {
                    digital_write(leds[i], LOW);
                }

                delay_msec(delay_ms);
            }
            status = BLINKY_SUCCESS;
        }
    }

    return status;
}

void blinky_end(void)
{
    number = 0;
    delay_msec = delay;
    pin_mode = pinMode;
    digital_write = digitalWrite;
}

static bool is_unique(uint8_t *pins, uint8_t number_of_leds)
{
    bool unique = true;

    for (uint8_t i = 0; unique && (i < number_of_leds); i++)
    {
        for (uint8_t j = i + 1; j < number_of_leds; j++)
        {
            if (pins[i] == pins[j])
            {
                unique = false;
                break;
            }
        }
    }

    return unique;
}