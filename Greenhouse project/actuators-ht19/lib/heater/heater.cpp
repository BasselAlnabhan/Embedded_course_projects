/**
 * @file heater.c
 * @author Valentine
 * @brief Implementation code for the Heater to be used in the Greenhouse project
 * @version 0.1
 * @date 2020-11-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <bsp_io.h>
#include <heater.h>
#include <stdlib.h>
#include <common.h>

#define SCALE (2.55f)

static void (*analog_write)(uint8_t, uint8_t) = bsp_analog_write;
static void (*pin_mode)(uint8_t, uint8_t) = bsp_pin_mode;

static uint8_t status = UNINITIALIZED;

void heater_init(heater_interface_t *heater_interface)
{
    if (heater_interface != NULL)
    {
        pin_mode = heater_interface->pin_mode;
        analog_write = heater_interface->analog_write;
    }

    pin_mode(HEATER_PIN, OUTPUT);
    analog_write(HEATER_PIN, 0);
    status = OKAY;
}

uint8_t heater_set_state(uint8_t value)
{
    if (status != UNINITIALIZED)
    {
        if ((value >= 0) && (value <= 100))
        {
            uint8_t val = (uint8_t)(SCALE * value);
            analog_write(HEATER_PIN, val);
            status = OKAY;
        }
        else
        {
            status = ERROR;
        }
    }

    return status;
}
