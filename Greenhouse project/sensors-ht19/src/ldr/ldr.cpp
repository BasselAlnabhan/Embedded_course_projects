#include <ldr.h>
#include <common.h>
#include <canbus.h>
#include <ldr_driver.h>

static uint8_t status = UNINITIALIZED;

void ldr_app_init(void)
{
    ldr_driver_init();
    status = OKAY;
}

void ldr_app_update(void)
{
    uint8_t ldr_value = 0U;

    if (status != UNINITIALIZED)
    {
        status = ldr_driver_read(&ldr_value);
    }

    set_light_intensity(ldr_value);
    set_light_intensity_sensor_status(status);
}