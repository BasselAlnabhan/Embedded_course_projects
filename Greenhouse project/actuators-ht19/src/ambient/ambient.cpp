/**
 * @file ambient.cpp
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief This is an app implementation to conrol the ambient system in the Greenhouse project. 
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <lamp.h>
#include <stdlib.h>
#include <canbus.h>
#include <heater.h>
#include <common.h>
#include <ambient.h>
#include <window_driver.h>
#include <air_fan_driver.h>

#define HIGH_AMBIENT (2U)
#define LOW_AMBIENT (0U)
#define MAX_PWM (100U)
#define MIN_PWM (0)

static uint8_t check_ambient_temperature_humdity(void);
static uint8_t check_ambient_light(void);

static uint8_t shineness = 0U;
static uint8_t heater_value = 0U;
static uint8_t check_ambient = OKAY;
static uint8_t check_ambient_light_value = OKAY;

static bool
validate_temperature_taget_values(void)
{
    return ((OKAY == validate_temperature_target_min()) &&
            (OKAY == validate_temperature_target_max()) &&
            (OKAY == validate_humidity_target_min()) &&
            (OKAY == validate_humidity_target_max()));
}

void ambient_init(void)
{
    lamp_init(NULL);
    air_fan_init(NULL);
    window_driver_init();
    heater_init(NULL);
}

void ambient_run(void)
{
    uint8_t fan_status = UNINITIALIZED, window_status = UNINITIALIZED, heater_status = UNINITIALIZED, lamp_status = UNINITIALIZED;

    // Temperature application
    if (validate_temperature_taget_values())
    {
        fan_status = OKAY, window_status = OKAY, heater_status = OKAY;
        if (OKAY == get_dht_sensor_status())
        {
            // Fan application control
            uint8_t state_ambient = check_ambient_temperature_humdity();

            if (!is_fans_state_overwritten())
            {
                switch (state_ambient)
                {
                case HIGH_AMBIENT:
                    fan_status = air_fan_turn_on();
                    set_fans_state(ON);
                    break;
                case LOW_AMBIENT:
                    fan_status = air_fan_turn_off();
                    set_fans_state(OFF);
                    break;
                case OKAY:
                    if (get_humidity() > get_humidity_target_max())
                    {
                        fan_status = air_fan_turn_on();
                        set_fans_state(ON);
                    }
                    if (get_humidity() < get_humidity_target_min())
                    {
                        fan_status = air_fan_turn_off();
                        set_fans_state(OFF);
                    }
                    break;
                default:
                    //Set fan in safemode
                    fan_status = air_fan_turn_off();
                    set_fans_state(OFF);
                    break;
                }
            }
            else
            {
                if (get_fans_state() == ON)
                {
                    fan_status = air_fan_turn_on();
                    set_fans_state(ON);
                }
                else
                {
                    fan_status = air_fan_turn_off();
                    set_fans_state(OFF);
                }
            }

            // window application control
            if (!is_window_state_overwritten())
            {
                switch (state_ambient)
                {
                case HIGH_AMBIENT:
                    window_status = window_driver_open();
                    set_window_state(OPEN);
                    break;
                case LOW_AMBIENT:
                    window_status = window_driver_close();
                    set_window_state(CLOSE);
                    break;
                case OKAY:
                    if (get_humidity() > get_humidity_target_max())
                    {
                        window_status = window_driver_open();
                        set_window_state(OPEN);
                    }
                    if (get_humidity() < get_humidity_target_min())
                    {
                        window_status = window_driver_close();
                        set_window_state(CLOSE);
                    }
                    break;
                default:
                    // Set window in safe mode
                    window_status = window_driver_close();
                    set_window_state(CLOSE);
                    break;
                }
            }
            else
            {
                if (OPEN == get_window_state())
                {
                    window_status = window_driver_open();
                    set_window_state(OPEN);
                }
                else
                {
                    window_status = window_driver_close();
                    set_window_state(CLOSE);
                }
            }

            // Heater appliation control
            if (!is_heater_warmness_overwritten())
            {
                switch (state_ambient)
                {
                case HIGH_AMBIENT:
                    if (heater_value != MIN_PWM)
                    {
                        heater_value--;
                    }
                    heater_status = heater_set_state(heater_value);
                    set_heater_warmness(heater_value);
                    break;
                case LOW_AMBIENT:
                    if (heater_value <= MAX_PWM)
                    {
                        heater_value++;
                    }
                    heater_status = heater_set_state(heater_value);
                    set_heater_warmness(heater_value);
                    break;
                case OKAY:
                    // Keep current state and do nothing
                    break;
                default:
                    // Set heater in safemode
                    heater_value = 0U;
                    heater_status = heater_set_state(OFF);
                    set_heater_warmness(OFF);
                }
            }
            else
            {
                uint8_t warmness = get_heater_warmness();
                heater_status = heater_set_state(warmness);
                set_heater_warmness(warmness);
            }
        }
        else
        {
            // If sensor error put system in safe mode (off state)
            fan_status = air_fan_turn_off();
            set_fans_state(OFF);

            window_status = window_driver_close();
            set_window_state(CLOSE);

            heater_value = 0U;
            heater_status = heater_set_state(OFF);
            set_heater_warmness(OFF);
        }
    }

    set_fans_status(fan_status);
    set_heater_status(heater_status);
    set_window_status(window_status);

    // light application
    if (OKAY == validate_light_intensity_target_min())
    {
        lamp_status = OKAY;
        if (OKAY == get_light_intensity_sensor_status())
        {
            if (!is_lamp_shininess_overwritten())
            {
                switch (check_ambient_light())
                {
                case HIGH_AMBIENT:
                    if (shineness != MIN_PWM)
                    {
                        shineness--;
                    }
                    lamp_status = lamp_set_state(shineness);
                    set_lamp_shininess(shineness);
                    break;
                case LOW_AMBIENT:
                    if (shineness <= MAX_PWM)
                    {
                        shineness++;
                    }
                    lamp_status = lamp_set_state(shineness);
                    set_lamp_shininess(shineness);
                    break;
                case OKAY:
                    // Do nothing and keep previous state
                    break;
                default:
                    // Set lamp in safemode (off state)
                    shineness = 0U;
                    lamp_status = lamp_set_state(OFF);
                    set_lamp_shininess(OFF);
                    break;
                }
            }
            else
            {
                uint8_t light = get_lamp_shininess();
                lamp_status = lamp_set_state(light);
                set_lamp_shininess(light);
            }
        }
        else
        {
            // Put the lamp in safemode (off state)
            shineness = 0U;
            lamp_status = lamp_set_state(OFF);
            set_lamp_shininess(OFF);
        }
    }

    set_lamp_status(lamp_status);
}

static uint8_t check_ambient_temperature_humdity(void)
{
    if (get_temperature() > get_temperature_target_max())
    {
        check_ambient = HIGH_AMBIENT;
    }
    else if (get_temperature() < get_temperature_target_min())
    {
        check_ambient = LOW_AMBIENT;
    }
    else
    {
        uint8_t avg_temperature = (float)((get_temperature_target_max() + get_temperature_target_min()) / 2);
        uint8_t avg_humidity = (float)((get_humidity_target_max() + get_humidity_target_min()) / 2);

        if ((avg_temperature <= get_temperature()) || (avg_humidity <= get_humidity()))
        {
            check_ambient = OKAY;
        }
    }

    return check_ambient;
}

static uint8_t check_ambient_light(void)
{
    uint8_t avg_light = (float)((get_light_intensity_target_min() + MAX_PWM) / 2);
    if (get_light_intensity() < get_light_intensity_target_min())
    {
        check_ambient_light_value = LOW_AMBIENT;
    }
    else if (avg_light < get_light_intensity())
    {
        check_ambient_light_value = HIGH_AMBIENT;
    }
    else
    {
        check_ambient_light_value = OKAY;
    }
    return check_ambient_light_value;
}