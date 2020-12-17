#include <bsp_io.h>
#include <stdlib.h>
#include <wl_driver.h>
#include <common.h>

#define REF_VOLTAGE (3.3f)
#define VOLTAGE_RESOLUTION (float)(REF_VOLTAGE / (1 << ADC_RESOLUTION))
#define R_REF (1500U)
#define MAX_CALIBRATION (1600U)
#define MIN_CALIBRATION (300U)
#define TO_LOW (2.30f)
#define TO_HIGH (21.50f)

static uint8_t status = UNINITIALIZED;

void driver_water_level_init()
{
    bsp_analog_read_resolution(ADC_RESOLUTION);
    status = OKAY;
}

uint8_t water_level_driver_read(uint8_t *length)
{
    if (status != UNINITIALIZED)
    {
        if (length == NULL)
        {
            status = ERROR;
        }
        else
        {
            uint16_t read_value = bsp_analog_read(WATER_LEVEL_PIN);

            float value = read_value * VOLTAGE_RESOLUTION;

            uint16_t resistance = (R_REF * value) / (REF_VOLTAGE - value);

            uint16_t map_value = MAX_CALIBRATION - resistance;
            *length = MAP(map_value, MIN_CALIBRATION, MAX_CALIBRATION, TO_LOW, TO_HIGH);
            status = OKAY;

            if ((resistance < MIN_CALIBRATION) || (resistance > MAX_CALIBRATION))
            {
                *length = 0U;
                status = ERROR;
            }
        }
    }
    return status;
}
