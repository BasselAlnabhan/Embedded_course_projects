#include <bsp.h>
#include <stdlib.h>
#include <blinky.h>

static bool is_unique(uint8_t *pins, uint8_t number_of_leds);

static uint8_t number;
static uint8_t leds[MAX_LEDS];

uint8_t blinky_begin(uint8_t *pins, uint8_t number_of_leds)
{
    blinky_end();

    uint8_t status = BLINKY_ERROR;

    if ((number_of_leds >= MIN_LEDS) && (number_of_leds <= MAX_LEDS) && (pins != NULL))
    {
        if (is_unique(pins, number_of_leds))
        {
            number = number_of_leds;

            for (uint8_t i = 0; i < number; i++)
            {
                leds[i] = pins[i];
                bsp_pin_mode(pins[i], OUTPUT);
                bsp_digital_write(pins[i], LOW);
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
                    bsp_digital_write(leds[i], HIGH);
                }

                bsp_delay(delay_ms);

                for (uint8_t i = 0; i < number; i++)
                {
                    bsp_digital_write(leds[i], LOW);
                }

                bsp_delay(delay_ms);
            }
            status = BLINKY_SUCCESS;
        }
    }

    return status;
}

void blinky_end(void)
{
    number = 0;
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