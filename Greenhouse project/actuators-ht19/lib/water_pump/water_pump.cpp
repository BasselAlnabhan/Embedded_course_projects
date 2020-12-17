/**
 * @file water_pump.cpp
 * @author Valentine (you@domain.com)
 * @brief Implementation code for the Wtaer Pump to be used in the Greenhouse project
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <common.h>
#include <bsp_io.h>
#include <stdlib.h>
#include <water_pump.h>

void (*pin_mode_t)(uint8_t pin, uint8_t mode) = bsp_pin_mode;
void (*digital_write_t)(uint8_t pin, uint8_t val) = bsp_digital_write;
uint8_t (*digital_read_t)(uint8_t pin) = bsp_digital_read;

static uint8_t status = UNINITIALIZED;

void water_pump_init(water_pump_interface_t *interface)
{
    if (interface != NULL)
    {
        pin_mode_t = interface->pin_mode;
        digital_read_t = interface->digital_read;
        digital_write_t = interface->digital_write;
    }

    pin_mode_t(WATER_PUMP_PIN, OUTPUT);
    digital_write_t(WATER_PUMP_PIN, LOW);
    status = OKAY;
}

uint8_t water_pump_turn_on(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write_t(WATER_PUMP_PIN, HIGH);
        status = (HIGH == digital_read_t(WATER_PUMP_PIN)) ? OKAY : ERROR;
    }

    return status;
}

uint8_t water_pump_turn_off(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write_t(WATER_PUMP_PIN, LOW);
        status = (LOW == digital_read_t(WATER_PUMP_PIN)) ? OKAY : ERROR;
    }

    return status;
}