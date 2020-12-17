/**
 * @file sm_app.cpp
 * @author Mohammed Fazlur Rahman (raseleee02@gmail.com)
 * @brief Appliction for soil  moisture
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <sm.h>
#include <canbus.h>
#include <common.h>
#include <sm_driver.h>

static uint8_t status = UNINITIALIZED;

void sm_app_init(void)
{
    soil_moisture_driver_init();
    status = OKAY;
}

void sm_app_update(void)
{
    uint8_t moisture = 0U;

    if (status != UNINITIALIZED)
    {
        status = soil_moisture_driver_read(&moisture);
    }
    set_soil_moisture(moisture);
    set_soil_moisture_sensor_status(status);
}