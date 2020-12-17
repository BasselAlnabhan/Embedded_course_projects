#include <canbus.h>
#include <common.h>
#include <watering.h>
#include <fm_driver.h>
#include <sm_driver.h>
#include <wl_driver.h>

#define CONTAINER (22U)
#define MIN_SENSOR_CONTAINER (1U)
#define MAX_PERCENT (85U)
#define CAN_INTERVAL (1000U)
#define PUMP_DELAY_ON (400U)
#define PUMP_DELAY_OFF (100U)
#define FLOW_MIN_VALUE (17U)
#define FLOW_MAX_VALUE (500U)

static uint8_t convert_to_percent(uint8_t water_level);
static bool set_can_values(void);
static uint8_t check_flow_meter(uint16_t flow, uint8_t status);
static void pump_state_delay(void);

static uint8_t can_counter = 0U;
static uint16_t pump_counter = 0U;
static uint16_t flow_rate_total = 0U;
static bool pump_state = false;

void watering_init(void)
{
    flow_meter_driver_init();
    driver_water_level_init();
    soil_moisture_driver_init();
}

void watering_run(void)
{
    can_counter++;

    uint8_t status = UNINITIALIZED;

    // Flow meter application
    uint16_t flow_rate = 0U;
    pump_state_delay();
    status = flow_meter_driver_read(&flow_rate);
    flow_rate_total += flow_rate;
    if (set_can_values())
    {
        status = check_flow_meter(flow_rate_total, status);
        if (status != UNINITIALIZED)
        {
            set_flow_rate(flow_rate_total);
        }
        set_flow_meter_sensor_status(status);
        flow_rate_total = 0U;

        // Water level appliation
        uint8_t percent = 0U;
        uint8_t water_level_length = 0U;
        status = water_level_driver_read(&water_level_length);
        if (status != UNINITIALIZED)
        {
            percent = convert_to_percent(water_level_length);
            if (percent > MAX_PERCENT)
            {
                status = ERROR;
            }
            set_water_level(percent);
        }
        set_water_level_sensor_status(status);

        // Soil moisture application
        uint8_t moisture = 0U;
        status = soil_moisture_driver_read(&moisture);
        if (status != UNINITIALIZED)
        {
            set_soil_moisture(moisture);
        }
        set_soil_moisture_sensor_status(status);
    }
}

static uint8_t convert_to_percent(uint8_t water_level)
{
    uint8_t temp = water_level + MIN_SENSOR_CONTAINER;
    uint8_t percent_level = ((float)temp / (float)CONTAINER) * 100;
    return percent_level;
}

static bool set_can_values(void)
{
    bool check = false;

    uint16_t temp = WATERING_INTERVAL * can_counter;
    if (temp == CAN_INTERVAL)
    {
        check = true;
        can_counter = 0U;
    }
    return check;
}

static uint8_t check_flow_meter(uint16_t flow, uint8_t status)
{
    if (pump_state)
    {
        if (flow < FLOW_MIN_VALUE || flow > FLOW_MAX_VALUE)
        {
            status = ERROR;
        }
    }
    return status;
}

static void pump_state_delay(void)
{
    // Function is called every 100ms
    pump_counter++;
    uint32_t state_time = pump_counter * WATERING_INTERVAL;

    if (ON == get_water_pump_state())
    {
        if (!pump_state)
        {
            pump_counter = 0U;
        }
        if ((state_time > PUMP_DELAY_ON) && (ON == get_water_pump_state()))
        {
            pump_state = true;
        }
    }
    else
    {
        if (pump_state)
        {
            pump_counter = 0U;
        }
        if ((state_time > PUMP_DELAY_OFF) && (OFF == get_water_pump_state()))
        {
            pump_state = false;
        }
    }
}