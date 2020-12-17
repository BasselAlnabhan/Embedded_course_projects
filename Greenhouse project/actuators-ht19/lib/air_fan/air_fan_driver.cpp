/**
 * @file air_fan.c
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief A special implementation to the Air-Fan to be used in the greenhouse project
 * @version 0.1
 * @date 2020-10-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <common.h>
#include <bsp_io.h>
#include <stdlib.h>
#include <air_fan_driver.h>

static void (*pin_mode)(uint8_t, uint8_t) = bsp_pin_mode;
static uint8_t (*digital_read)(uint8_t) = bsp_digital_read;
static void (*digital_write)(uint8_t, uint8_t) = bsp_digital_write;

static uint8_t status = UNINITIALIZED;

void air_fan_init(fan_interfc_t *interface)
{
    if (interface != NULL)
    {
        pin_mode = interface->pin_mode;
        digital_read = interface->digital_read;
        digital_write = interface->digital_write;
    }

    pin_mode(FAN_PIN, OUTPUT);
    digital_write(FAN_PIN, LOW);
    status = OKAY;
}

uint8_t air_fan_turn_on(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write(FAN_PIN, HIGH);
        status = (HIGH == digital_read(FAN_PIN)) ? OKAY : ERROR;
    }

    return status;
}

uint8_t air_fan_turn_off(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write(FAN_PIN, LOW);
        status = (LOW == digital_read(FAN_PIN)) ? OKAY : ERROR;
    }

    return status;
}
