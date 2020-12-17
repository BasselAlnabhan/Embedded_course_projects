#include <wl.h>
#include <common.h>
#include <canbus.h>
#include <wl_driver.h>

#define CONTAINER (22U)
#define MIN_SENSOR_CONTAINER (2U)
#define MAX_PERCENT (85U)

uint8_t static status = UNINITIALIZED;
static uint8_t convert_to_percent(uint8_t water_level);

void water_level_app_init()
{
    driver_water_level_init();
    status = OKAY;
}
void water_level_app_update(void)
{
    uint8_t percent = 0U;
    if (status != UNINITIALIZED)
    {
        uint8_t water_level_length = 0U;
        status = water_level_driver_read(&water_level_length);
        percent = convert_to_percent(water_level_length);
        if (percent > MAX_PERCENT)
        {
            status = ERROR;
        }
    }

    set_water_level_sensor_status(status);
    set_water_level(percent);
}
static uint8_t convert_to_percent(uint8_t water_level)
{
    uint8_t temp = water_level + MIN_SENSOR_CONTAINER;
    float percent_level = ((float)temp / (float)CONTAINER) * 100;
    return percent_level;
}