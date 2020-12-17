#include <DHT11.h>
#include <canbus.h>
#include <common.h>
#include <ambient.h>
#include <ldr_driver.h>

static bool validate_calibration_values(void)
{
    return ((OKAY == validate_temperature_calibration_min()) &&
            (OKAY == validate_temperature_calibration_max()) &&
            (OKAY == validate_humidity_calibration_min()) &&
            (OKAY == validate_humidity_calibration_max()));
}

void ambient_init(void)
{
    dht11_init();
    ldr_driver_init();
}

void ambient_run(void)
{
    uint8_t status = UNINITIALIZED;

    if (validate_calibration_values())
    {
        uint8_t humidity = 0;
        float temperature = 0.0f;
        status = dht11_read(&temperature, &humidity);

        if (status != DHT11_WAIT)
        {
            if (status == DHT11_OKAY)
            {
                if ((temperature < get_temperature_calibration_min()) || (temperature > get_temperature_calibration_max()))
                {
                    status = DHT11_ERROR;
                }
                else if ((humidity < get_humidity_calibration_min()) || (humidity > get_humidity_calibration_max()))
                {
                    status = DHT11_ERROR;
                }
                else
                {
                    set_temperature(temperature);
                    set_humidity(humidity);
                }
            }

            status = (status == DHT11_OKAY) ? OKAY : ERROR;
            set_dht_sensor_status(status);
        }
    }
    else
    {
        set_dht_sensor_status(status);
    }

    uint8_t intensity = 0U;
    status = ldr_driver_read(&intensity);
    if (status != UNINITIALIZED)
    {
        set_light_intensity(intensity);
    }
    set_light_intensity_sensor_status(status);
}