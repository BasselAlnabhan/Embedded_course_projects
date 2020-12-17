#include <common.h>
#include <bsp_io.h>
#include <stdlib.h>
#include <water_valve.h>

static uint8_t status = UNINITIALIZED;

void (*pin_mode)(uint8_t pin, uint8_t mode) = bsp_pin_mode;
void (*digital_write)(uint8_t pin, uint8_t val) = bsp_digital_write;
uint8_t (*digital_read)(uint8_t pin) = bsp_digital_read;

void water_valve_init(valve_interface_t *interface)
{
    if (interface != NULL)
    {
        pin_mode = interface->pin_mode;
        digital_write = interface->digital_write;
        digital_read = interface->digital_read;
    }

    pin_mode(VALVE_PIN, OUTPUT);
    digital_write(VALVE_PIN, LOW);
    status = OKAY;
}

uint8_t water_valve_set_state_on(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write(VALVE_PIN, HIGH);
        status = (HIGH == digital_read(VALVE_PIN)) ? OKAY : ERROR;
    }

    return status;
}

uint8_t water_valve_set_state_off(void)
{
    if (status != UNINITIALIZED)
    {
        digital_write(VALVE_PIN, LOW);
        status = (LOW == digital_read(VALVE_PIN)) ? OKAY : ERROR;
    }

    return status;
}
