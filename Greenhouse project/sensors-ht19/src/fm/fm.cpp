/**
 * @file fm.cpp
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief Implementation of the application for flow meter sensor
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <fm.h>
#include <canbus.h>
#include <common.h>
#include <fm_driver.h>

static uint8_t status = UNINITIALIZED;

void fm_app_init(void)
{
    flow_meter_driver_init();
    status = OKAY;
}

void fm_app_update(void)
{
    uint16_t flow_rate = 0U;

    if (status != UNINITIALIZED)
    {
        status = flow_meter_driver_read(&flow_rate);
    }

    set_flow_rate(flow_rate);
    set_flow_meter_sensor_status(status);
}