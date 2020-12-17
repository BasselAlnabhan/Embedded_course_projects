#include <bsp_io.h>
#include <stdlib.h>
#include <common.h>
#include <ldr_driver.h>

// The 4 makros below are used for the convert process in the "ldr_driver_read()"
#define CALIBRATION_DARK (994U) // The upper boundary of the current range.
#define CALIBRATION_LIGHT (0U)  // The lower boundary of the current range.
#define TO_LOW (0U)             // The lower boundary of the target range.
#define TO_HIGH (100U)          // The upper boundary of the target range.

#define LDR_PIN 21

static uint8_t status = UNINITIALIZED;

void ldr_driver_init(void)
{
    bsp_analog_read_resolution(ADC_RESOLUTION);
    status = OKAY;
}

uint8_t ldr_driver_read(uint8_t *ldr_value)
{
    if (status != UNINITIALIZED)
    {
        if (ldr_value == NULL)
        {
            status = ERROR;
        }
        else
        {
            uint16_t read_value = bsp_analog_read(LDR_PIN);

            if (read_value < CALIBRATION_LIGHT || read_value > CALIBRATION_DARK)
            {
                status = ERROR;
            }
            else
            {
                uint16_t map_value = CALIBRATION_DARK - read_value;

                *ldr_value = MAP(map_value, CALIBRATION_LIGHT, CALIBRATION_DARK, TO_LOW, TO_HIGH);
                status = OKAY;
            }
        }
    }

    return status;
}