/**
 * @file watering.cpp
 * @author Stefan Edenholm (stefan.edenholm@yaelev.se)
 * @brief This is an app implementation to control the watering system in the Greenhouse project.
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdlib.h> // NULL
#include <canbus.h>
#include <common.h>
#include <watering.h>
#include <water_valve.h>
#include <water_pump.h>
#include <bsp.h>

#define WATERING_HIGH_VALUE (1U)
#define WATERING_LOW_VALUE (0U)
#define WATERING_OKAY (2U)
#define MILLISEC (1000U)                // one second in milliseconds
#define MAX_WATER_PER_RUN (2000U)       // in milliliter
#define PUMP_SLEEP_INTERVAL (10000U)    // in milliseconds
//#define DEBUG

#if (MILLISEC % WATERING_INTERVAL != 0)
#error "MILLISEC shall be divisiable by WATERING_INTERVAL"
#endif

static uint8_t check_water_valve = WATERING_OKAY;
static uint8_t check_water_pump = WATERING_OKAY;
static uint8_t pump_counter = 0U;
static uint16_t water_consumed = 0U;
static uint16_t pump_counter_limit = (uint16_t)(MILLISEC / WATERING_INTERVAL);
static bool pump_sleep = false;
static uint32_t pump_sleep_time = 0U;

static uint8_t check_watering_water_valve(void);
static uint8_t check_watering_water_pump(void);
static void water_valve_app_control(void);
static void water_pump_app_control(void);
static void check_water_consumed(void);

void watering_init(void)
{
    water_valve_init(NULL);
    water_pump_init(NULL);
    pump_counter = 0U;
    water_consumed = 0U;
    pump_sleep = false;
    pump_sleep_time = 0U;
}

void watering_run(void)
{
    // Water valve application control
    water_valve_app_control();

    // Water pump application control
    if (pump_sleep)
    {
        pump_sleep_time += WATERING_INTERVAL;
        pump_sleep = (pump_sleep_time >= PUMP_SLEEP_INTERVAL) ? false : true;
    }

    if (!pump_sleep)
    {
        water_pump_app_control();
    }
}

static void water_valve_app_control(void)
{
    uint8_t water_valve_status = UNINITIALIZED;

    if ((validate_water_level_target_min() == OKAY) && (validate_water_level_target_max() == OKAY))
    {
        if (get_water_level_sensor_status() == OKAY)
        {
            if (is_water_valve_state_overwritten() == NO)
            {
                switch (check_watering_water_valve())
                {
                    case WATERING_HIGH_VALUE:
                        water_valve_status = water_valve_set_state_off();
                        set_water_valve_state(OFF);
                        break;
                    case WATERING_LOW_VALUE:
                        water_valve_status = water_valve_set_state_on();
                        set_water_valve_state(ON);
                        break;
                    case WATERING_OKAY:
                        water_valve_status = water_valve_set_state_off();
                        set_water_valve_state(OFF);
                        break;
                    default:
                        // Set water valve in safemode
                        water_valve_status = water_valve_set_state_off();
                        set_water_valve_state(OFF);
                        break;
                }
            }
            else
            {
                if (get_water_valve_state() == ON)
                {
                    water_valve_status = water_valve_set_state_on();
                    set_water_valve_state(ON);
                }
                else
                {
                    water_valve_status = water_valve_set_state_off();
                    set_water_valve_state(OFF);
                }
            }
        }
        else
        {
            // get_water_level_sensor_status() != OKAY
            // Set water valve in safemode (off state)
            water_valve_status = water_valve_set_state_off();
            set_water_valve_state(OFF);
        }
    }

    set_water_valve_status(water_valve_status);
}

static void water_pump_app_control(void)
{
    uint8_t water_pump_status = UNINITIALIZED;

    if ((validate_soil_moisture_target_min() == OKAY) && (validate_soil_moisture_target_max() == OKAY))
    {
        if (get_soil_moisture_sensor_status() == OKAY)
        {
            if (is_water_pump_state_overwritten() == NO)
            {
                switch (check_watering_water_pump())
                {
                    case WATERING_HIGH_VALUE:
                        water_pump_status = water_pump_turn_off();
                        set_water_pump_state(OFF);
                        break;
                    case WATERING_LOW_VALUE:
                        check_water_consumed();
                        if (pump_sleep)
                        {
                            water_pump_status = water_pump_turn_off();
                            set_water_pump_state(OFF);
                        }
                        else
                        {
                            water_pump_status = water_pump_turn_on();
                            set_water_pump_state(ON);
                        }
                        break;
                    case WATERING_OKAY:
                        water_pump_status = water_pump_turn_off();
                        set_water_pump_state(OFF);
                        break;
                    default:
                        // Set water pump in safemode
                        water_pump_status = water_pump_turn_off();
                        set_water_pump_state(OFF);
                        break;
                }
            }
            else
            {
                if (get_water_pump_state() == ON)
                {
                    water_pump_status = water_pump_turn_on();
                    set_water_pump_state(ON);
                }
                else
                {
                    water_pump_status = water_pump_turn_off();
                    set_water_pump_state(OFF);
                }
            }
        }
        else
        {
            // get_soil_moisture_sensor_status() != OKAY
            // Set water pump in safemode (off state)
            water_pump_status = water_pump_turn_off();
            set_water_pump_state(OFF);
        }
    }

    set_water_pump_status(water_pump_status);
}

static uint8_t check_watering_water_valve(void)
{
    if (get_water_level() > get_water_level_target_max())
    {
        check_water_valve = WATERING_HIGH_VALUE;
    }
    else if (get_water_level() < get_water_level_target_min())
    {
        check_water_valve = WATERING_LOW_VALUE;
    }
    else
    {
        uint8_t avg_water_level_target = (uint8_t)((get_water_level_target_min() + get_water_level_target_max()) / 2.0);

        if (get_water_level() >= avg_water_level_target)
        {
            check_water_valve = WATERING_OKAY;
        }
    }

    return check_water_valve;
}

static uint8_t check_watering_water_pump(void)
{
    if (get_soil_moisture() > get_soil_moisture_target_max())
    {
        check_water_pump = WATERING_HIGH_VALUE;
    }
    else if (get_soil_moisture() < get_soil_moisture_target_min())
    {
        check_water_pump = WATERING_LOW_VALUE;
    }
    else
    {
        uint8_t avg_soil_moisture_target = (uint8_t)((get_soil_moisture_target_min() + get_soil_moisture_target_max()) / 2.0);

        if (get_soil_moisture() >= avg_soil_moisture_target)
        {
            check_water_pump = WATERING_OKAY;
        }
    }

    return check_water_pump;
}

static void check_water_consumed(void)
{
    pump_counter = (pump_counter + 1) % pump_counter_limit;

    #ifdef DEBUG
    PRINTF("Pump counter: %d\n", pump_counter);
    #endif

    if (pump_counter == 0 && get_flow_meter_sensor_status() == OKAY)
    {
        water_consumed += get_flow_rate();

        #ifdef DEBUG
        PRINTF("Water consumed: %d\n", water_consumed);
        #endif
    }

    if (water_consumed >= MAX_WATER_PER_RUN)
    {
        water_consumed = 0U;
        pump_sleep = true;

        #ifdef DEBUG
        PRINTF("%s\n", "Pump is sleeping");
        #endif

        pump_sleep_time = 0U;
    }
}
