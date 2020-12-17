/**
 * @file system_status.cpp
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <bsp.h>
#include <canbus.h>
#include <candata.h>
#include <rgb_driver.h>
#include <buzzer_driver.h>
#include <system_status.h>
#include <math.h>

#define TEMPO (160) //speed of the buzzer melody

interface_t *rgb_interface = {NULL};

void system_status_init(void)
{
    buzzer_driver_init();
    rgb_driver_init(rgb_interface);
}

static uint8_t get_final_system_status(void)
{
    data_t data = get_candata();

    if ((data.sensor.flow_meter.status == ERROR) ||
        (data.sensor.humidity.status == ERROR) ||
        (data.sensor.light.status == ERROR) ||
        (data.sensor.moisture.status == ERROR) ||
        (data.sensor.temperature.status == ERROR) ||
        (data.sensor.water_level.status == ERROR) ||
        (data.actuator.heater.status == ERROR) ||
        (data.actuator.fans.status == ERROR) ||
        (data.actuator.window.status == ERROR) ||
        (data.actuator.lamp.status == ERROR) ||
        (data.actuator.water_pump.status == ERROR) ||
        (data.actuator.water_valve.status == ERROR) ||
        (data.hmi.eeprom.status == ERROR) ||
        (data.hmi.keypad.status == ERROR) ||
        (data.hmi.rgb.status == ERROR) ||
        (data.hmi.display.status == ERROR) ||
        (data.hmi.buzzer.status == ERROR) ||
        (data.communication.sdcard.status == ERROR) ||
        (data.communication.terminal.status == ERROR) ||
        ((data.communication.esp32.status != OKAY) && (data.communication.esp32.status != UNINITIALIZED)))
    {
        return ERROR;
    }

    if ((data.sensor.flow_meter.status == UNINITIALIZED) ||
        (data.sensor.humidity.status == UNINITIALIZED) ||
        (data.sensor.light.status == UNINITIALIZED) ||
        (data.sensor.moisture.status == UNINITIALIZED) ||
        (data.sensor.temperature.status == UNINITIALIZED) ||
        (data.sensor.water_level.status == UNINITIALIZED) ||
        (data.communication.rtc.status == UNINITIALIZED) ||
        (data.communication.esp32.status == UNINITIALIZED) ||
        (data.actuator.fans.status == UNINITIALIZED) ||
        (data.actuator.heater.status == UNINITIALIZED) ||
        (data.actuator.lamp.status == UNINITIALIZED) ||
        (data.actuator.water_pump.status == UNINITIALIZED) ||
        (data.actuator.water_valve.status == UNINITIALIZED) ||
        (data.actuator.window.status == UNINITIALIZED) ||
        (data.hmi.buzzer.status == UNINITIALIZED) ||
        (data.hmi.display.status == UNINITIALIZED) ||
        (data.hmi.keypad.status == UNINITIALIZED) ||
        (data.hmi.rgb.status == UNINITIALIZED) ||
        (data.hmi.eeprom.status == UNINITIALIZED))
    {
        return WARNING;
    }

    return OKAY;
}

void system_status_run(void)
{
    uint8_t system_status = get_final_system_status();
    uint32_t temp = millis();

    switch (system_status)
    {
    case ERROR:
        // Set the color of the LED to RED
        rgb_driver_red_light();
        // Set the buzzer to what you want
        if ((temp % 1000 >= 0) && (temp % 1000 <= 600))
        {
            buzzer_driver_tone(NOTE_AS5, 8);
        }
        break;

    case WARNING:
        // Set the color of the LED to Yellow
        rgb_driver_yellow_light();
        // Set the buzzer to what you want
        if ((temp % 3000 >= 0) && (temp % 3000 <= 200))
        {
            buzzer_driver_tone(NOTE_C1, 6);
        }
        break;

    default:
        // Set the color of the LED to Green
        rgb_driver_green_light();
        break;
    }
    set_system_status(system_status);
}

// TERMINAL ERROR = ERROR, CONNECTED/DISCONNECTED = OKAY
// ESP32 = UNINITIALIZED = WARNING, OKAY = OKAY, OTHERWISE = ERROR