#include <lamp.h>
#include <common.h>
#include <bsp_io.h>
#include <stdlib.h>

#define SCALE (2.55f)

static void (*analog_write)(uint8_t, uint8_t) = bsp_analog_write;
static void (*pin_mode)(uint8_t, uint8_t) = bsp_pin_mode;

static int8_t status = UNINITIALIZED;

void lamp_init(lamp_interface_t *interface)
{
    if (interface != NULL)
    {
        analog_write = interface->bsp_analog_write;
        pin_mode = interface->bsp_pin_mode;
    }

    pin_mode(LAMP_PIN, OUTPUT);
    analog_write(LAMP_PIN, 0);
    status = OKAY;
}

uint8_t lamp_set_state(uint8_t value)
{
    if (status != UNINITIALIZED)
    {
        if ((value >= 0) && (value <= 100))
        {
            uint8_t val = (uint8_t)(SCALE * value);
            analog_write(LAMP_PIN, val);
            status = OKAY;
        }
        else
        {
            status = ERROR;
        }
    }

    return status;
}
