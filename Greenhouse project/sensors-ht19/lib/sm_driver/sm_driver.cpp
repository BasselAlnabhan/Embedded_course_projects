/**
 * @file sm_driver.cpp
 * @author Mohammed Fazlur Rahman (raseleee02@gmail.com)
 * @brief Soil moisture driver
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <bsp_io.h>
#include <stdlib.h>
#include <common.h>
#include <sm_driver.h>

#define CALIBRATION_DRY (920U)
#define CALIBRATION_WET (300U)
#define SOIL_MOISTURE_PIN (14)
#define OUT_MIN (0U)
#define OUT_MAX (100U)

static uint8_t status = UNINITIALIZED;

void soil_moisture_driver_init()
{
    bsp_analog_read_resolution(ADC_RESOLUTION);
    status = OKAY;
}

uint8_t soil_moisture_driver_read(uint8_t *moisture)
{
    if (status != UNINITIALIZED)
    {
        if (moisture == NULL)
        {
            status = ERROR;
        }
        else
        {
            uint16_t read_value = bsp_analog_read(SOIL_MOISTURE_PIN);

            if (read_value > CALIBRATION_DRY || read_value < CALIBRATION_WET)
            {
                status = ERROR;
            }
            else
            {
                uint16_t map_value = CALIBRATION_DRY - read_value;

                *moisture = MAP(map_value, CALIBRATION_WET, CALIBRATION_DRY, OUT_MIN, OUT_MAX);
                status = OKAY;
            }
        }
    }
    return status;
}