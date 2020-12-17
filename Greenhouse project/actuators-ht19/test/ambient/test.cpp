/**
 * @file test.cpp
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief Tests for actuator ambient application
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifdef TARGET
#include <Arduino.h>
#include <Servo.h>
static Servo motor;
#endif

#include <unity.h>
#include <ambient.h>
#include <common.h>
#include <canbus.h>
#include "lamp.h"
#include "heater.h"
#include "window_driver.h"
#include "air_fan_driver.h"
#include <bsp.h>

// Lamp driver
static uint8_t lamp_shininess = 0;
static uint8_t lamp_status = UNINITIALIZED;
void lamp_init(lamp_interface_t *interface)
{
    lamp_status = OKAY;
#ifdef TARGET
    pinMode(LAMP_PIN, OUTPUT);
#endif
}
uint8_t lamp_set_state(uint8_t value)
{
    if (lamp_status != UNINITIALIZED)
    {
        if (value <= 100 && value >= 0)
        {
#ifdef TARGET
            analogWrite(LAMP_PIN, value);
#endif
            lamp_shininess = value;
            lamp_status = OKAY;
        }
        else
            lamp_status = ERROR;
    }
    return lamp_status;
}

// window driver
static uint8_t win_state = CLOSE;
static uint8_t win_status = UNINITIALIZED;
uint8_t window_driver_init(void)
{
#ifdef TARGET
    motor.attach(WINDOW_PIN);
#endif
    win_status = OKAY;
    return win_status;
}
uint8_t window_driver_open(void)
{
#ifdef TARGET
    if (!motor.attached())
        return ERROR;
    motor.write(90);
#endif
    win_state = OPEN;
    win_status = OKAY;
    return win_status;
}
uint8_t window_driver_close(void)
{
#ifdef TARGET
    if (!motor.attached())
        return ERROR;
    motor.write(0);
#endif
    win_state = CLOSE;
    win_status = OKAY;
    return win_status;
}

// heater driver
static uint8_t heater_warmness = OFF;
static uint8_t heater_status = UNINITIALIZED;
void heater_init(heater_interface_t *heater_interface)
{
    heater_status = OKAY;
#ifdef TARGET
    pinMode(HEATER_PIN, OUTPUT);
#endif
}
uint8_t heater_set_state(uint8_t value)
{
    if (heater_status != UNINITIALIZED)
    {
        if (value <= 100 && value >= 0)
        {
#ifdef TARGET
            analogWrite(HEATER_PIN, value);
#endif
            heater_warmness = value;
            heater_status = OKAY;
        }
        else
            heater_status = ERROR;
    }
    return heater_status;
}

// fan driver
static uint8_t fan_state = OFF;
static uint8_t fan_status = UNINITIALIZED;
void air_fan_init(fan_interfc_t *interface)
{
#ifdef TARGET
    pinMode(FAN_PIN, OUTPUT);
#endif
    fan_state = OFF;
    fan_status = OKAY;
}
uint8_t air_fan_turn_on(void)
{
#ifdef TARGET
    digitalWrite(FAN_PIN, HIGH);
#endif
    fan_state = ON;
    fan_status = OKAY;
    return fan_status;
}
uint8_t air_fan_turn_off(void)
{
#ifdef TARGET
    digitalWrite(FAN_PIN, LOW);
#endif
    fan_state = OFF;
    fan_status = OKAY;
    return fan_status;
}

// TESTS
void setUp(void) {}
void tearDown(void) {}

void test_non_valid_target_calibration_values(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));

    // test no valid calibration values
    TEST_ASSERT_FALSE(set_test_temperature_target_min(51));
    TEST_ASSERT_FALSE(set_test_temperature_target_max(51));
    TEST_ASSERT_FALSE(set_test_humidity_target_min(101));
    TEST_ASSERT_FALSE(set_test_humidity_target_max(101));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_window_status());

    // test temperature_target_min_valid
    TEST_ASSERT_TRUE(set_test_temperature_target_min(50));
    TEST_ASSERT_FALSE(set_test_temperature_target_max(51));
    TEST_ASSERT_FALSE(set_test_humidity_target_min(101));
    TEST_ASSERT_FALSE(set_test_humidity_target_max(101));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_window_status());

    // test temperature target min and max is valid
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_FALSE(set_test_humidity_target_min(101));
    TEST_ASSERT_FALSE(set_test_humidity_target_max(101));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_window_status());

    // test temperature target min and max is valid and humidity target min is valid
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_FALSE(set_test_humidity_target_max(101));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_window_status());

    // test all values are valid
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());
}

void test_dht_error_safe_mode(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));

    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));

    // Normal system run
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());

    // DHT sensor error occurs
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(ERROR));
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_dht_sensor_status());

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());
}

void test_fan_is_overwritten(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));

    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));

    // Normal system run
    overwrite_test_fans_state(NO);
    overwrite_test_heater_warmness(NO);
    overwrite_test_window_state(NO);
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());

    // Overwrite the fan state and set the fan to be OFF
    overwrite_test_fans_state(YES);
    TEST_ASSERT_TRUE(set_test_fans_state(OFF));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());
}

void test_fan_logic(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));
    overwrite_test_fans_state(NO);

    // Test temperature above target max
    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature same as target max (OKAY state) and humidity below target_min
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(10));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature same as target max (OKAY state) and humidity within range
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature same as target max (OKAY state) and humidity above target_max
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(85));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature same as target min, humidity in range
    TEST_ASSERT_TRUE(set_test_temperature(20));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature same as target min, humidity below target min
    TEST_ASSERT_TRUE(set_test_temperature(20));
    TEST_ASSERT_TRUE(set_test_humidity(19));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Test temperature below target min
    TEST_ASSERT_TRUE(set_test_temperature(19));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());

    // Temperature increase to above target max again
    TEST_ASSERT_TRUE(set_test_temperature(31));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
}

void test_window_is_overwritten(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));

    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));

    // Normal system run
    overwrite_test_fans_state(NO);
    overwrite_test_heater_warmness(NO);
    overwrite_test_window_state(NO);
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());

    // Overwrite the window state to be closed
    overwrite_test_window_state(YES);
    TEST_ASSERT_TRUE(set_test_window_state(CLOSE));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());
}

void test_window_logic(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));
    overwrite_test_window_state(NO);

    // Test temperature above target max
    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature same as target max (OKAY state) and humidity below target_min
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(10));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature same as target max (OKAY state) and humidity within range
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature same as target max (OKAY state) and humidity above target_max
    TEST_ASSERT_TRUE(set_test_temperature(30));
    TEST_ASSERT_TRUE(set_test_humidity(85));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature same as target min, humidity in range
    TEST_ASSERT_TRUE(set_test_temperature(20));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature same as target min, humidity below target min
    TEST_ASSERT_TRUE(set_test_temperature(20));
    TEST_ASSERT_TRUE(set_test_humidity(19));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Test temperature below target min
    TEST_ASSERT_TRUE(set_test_temperature(19));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(CLOSE, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    // Temperature increase to above target max again
    TEST_ASSERT_TRUE(set_test_temperature(31));
    TEST_ASSERT_TRUE(set_test_humidity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());
}

void test_heater_is_overwritten(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));

    TEST_ASSERT_TRUE(set_test_temperature(35));
    TEST_ASSERT_TRUE(set_test_humidity(50));

    // Normal system run
    overwrite_test_fans_state(NO);
    overwrite_test_heater_warmness(NO);
    overwrite_test_window_state(NO);
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_fans_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_window_status());

    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(0, get_test_heater_warmness());

    // Overwrite the heater state
    overwrite_test_heater_warmness(YES);
    TEST_ASSERT_TRUE(set_test_heater_warmness(20));

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OPEN, get_test_window_state());
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_fans_state());
    TEST_ASSERT_EQUAL_UINT8(20, get_test_heater_warmness());
}

void test_heater_logic(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_dht_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_temperature_target_min(20));
    TEST_ASSERT_TRUE(set_test_temperature_target_max(30));
    TEST_ASSERT_TRUE(set_test_humidity_target_min(20));
    TEST_ASSERT_TRUE(set_test_humidity_target_max(80));
    overwrite_test_heater_warmness(NO);

    // Test temperature below min target temperature
    TEST_ASSERT_TRUE(set_test_temperature(18));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(1, get_test_heater_warmness());

    // Test warmness increase with 1% per call of ambient run if temperature is below target min
    ambient_run();
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(3, get_test_heater_warmness());

    // Test heater warmness keep increasing till avarage of target min and max is reached
    TEST_ASSERT_TRUE(set_test_temperature(22));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(4, get_test_heater_warmness());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());

    // Test heater warmness is kept when avarage target temperature is achived
    TEST_ASSERT_TRUE(set_test_temperature(25));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(4, get_test_heater_warmness());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());

    // Test heater warmness reduced by 1% when temperature drops towards min target temperature
    TEST_ASSERT_TRUE(set_test_temperature(22));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(4, get_test_heater_warmness());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());

    // Test heater warmness increased by 1% when temperature drops below min target temperature
    TEST_ASSERT_TRUE(set_test_temperature(19));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(5, get_test_heater_warmness());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());

    // Test heater warmness decreased by 1% when temperature is above max target
    TEST_ASSERT_TRUE(set_test_temperature(31));
    TEST_ASSERT_TRUE(set_test_humidity(30));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(4, get_test_heater_warmness());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_heater_status());
}

void test_light_non_valid_calibration(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_light_intensity_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_light_intensity(60));

    // Non valid target value
    TEST_ASSERT_FALSE(set_test_light_intensity_target_min(101));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_lamp_status());

    // Valid target value
    TEST_ASSERT_TRUE(set_test_light_intensity_target_min(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
}

void test_light_sensor_error_safe_mode()
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_light_intensity_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_light_intensity_target_min(50));
    TEST_ASSERT_TRUE(set_test_light_intensity(40));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
    TEST_ASSERT_EQUAL_UINT8(1, get_test_lamp_shininess());

    // Sensor error occur
    TEST_ASSERT_TRUE(set_test_light_intensity_sensor_status(ERROR));
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_light_intensity_sensor_status());
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_lamp_shininess());
}

void test_shininess_is_overwritten(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_light_intensity_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_light_intensity_target_min(50));
    TEST_ASSERT_TRUE(set_test_light_intensity(40));
    overwrite_test_lamp_shininess(NO);

    // Normal system run
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
    TEST_ASSERT_EQUAL_UINT8(1, get_test_lamp_shininess());

    // Overwrite the shininess
    overwrite_test_lamp_shininess(YES);
    set_test_lamp_shininess(20);

    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
    TEST_ASSERT_EQUAL_UINT8(20, get_test_lamp_shininess());
}

void test_light_logic(void)
{
    ambient_init();
    TEST_ASSERT_TRUE(set_test_light_intensity_sensor_status(OKAY));
    TEST_ASSERT_TRUE(set_test_light_intensity_target_min(50));
    overwrite_test_lamp_shininess(NO);

    // Test the lamps shininess has increased with 1% per call of ambient_run() when ldr value is below target value
    TEST_ASSERT_TRUE(set_test_light_intensity(40));
    ambient_run();
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(3, get_test_lamp_shininess()); // The value were 1 from previous test
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());

    // Test the lamps shininess is kept when target min is archived
    TEST_ASSERT_TRUE(set_test_light_intensity(50));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(3, get_test_lamp_shininess());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());

    // Test the lamps shininess is reduced when light intensity is above avarage light ((light_intensity_target_min() + 100) / 2)
    TEST_ASSERT_TRUE(set_test_light_intensity(80));
    ambient_run();
    TEST_ASSERT_EQUAL_UINT8(2, get_test_lamp_shininess());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_lamp_status());
}

#ifdef TARGET

void loop()
{
}

void setup()
{
    delay(3000);
#else

int main(void)
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_non_valid_target_calibration_values);
    RUN_TEST(test_dht_error_safe_mode);
    RUN_TEST(test_fan_is_overwritten);
    RUN_TEST(test_fan_logic);
    RUN_TEST(test_window_is_overwritten);
    RUN_TEST(test_window_logic);
    RUN_TEST(test_heater_is_overwritten);
    RUN_TEST(test_heater_logic);
    RUN_TEST(test_light_non_valid_calibration);
    RUN_TEST(test_light_sensor_error_safe_mode);
    RUN_TEST(test_shininess_is_overwritten);
    RUN_TEST(test_light_logic);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}