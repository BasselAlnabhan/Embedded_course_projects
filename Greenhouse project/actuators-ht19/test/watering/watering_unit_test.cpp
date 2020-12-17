/**
 * @file watering_unit_test.cpp
 * @author Stefan Edenholm (stefan.edenholm@yaelev.se)
 * @brief Tests for actuator watering application
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <canbus.h>
#include <common.h>
#include <watering.h>
#include "water_valve.h"
#include "water_pump.h"
#include <stdio.h>

//#define DEBUG

// Water valve driver
static uint8_t water_valve_status = UNINITIALIZED;
void water_valve_init(valve_interface_t *interface)
{
    #ifdef TARGET
        pinMode(VALVE_PIN, OUTPUT);
    #endif

    water_valve_status = OKAY;
}

uint8_t water_valve_set_state_on(void)
{
    if (water_valve_status != UNINITIALIZED)
    {
        #ifdef TARGET
            digitalWrite(VALVE_PIN, HIGH);
        #endif

        water_valve_status = OKAY;
    }

    return water_valve_status;
}

uint8_t water_valve_set_state_off(void)
{
    if (water_valve_status != UNINITIALIZED)
    {
        #ifdef TARGET
            digitalWrite(VALVE_PIN, LOW);
        #endif

        water_valve_status = OKAY;
    }

    return water_valve_status;
}

// Water pump driver
static uint8_t water_pump_status = UNINITIALIZED;
void water_pump_init(water_pump_interface_t *interface)
{
    #ifdef TARGET
        pinMode(WATER_PUMP_PIN, OUTPUT);
    #endif

    water_pump_status = OKAY;
}

uint8_t water_pump_turn_on(void)
{
    if (water_pump_status != UNINITIALIZED)
    {
        #ifdef TARGET
            digitalWrite(WATER_PUMP_PIN, HIGH);
        #endif

        water_pump_status = OKAY;
    }

    return water_pump_status;
}

uint8_t water_pump_turn_off(void)
{
    if (water_pump_status != UNINITIALIZED)
    {
        #ifdef TARGET
            digitalWrite(WATER_PUMP_PIN, LOW);
        #endif

        water_pump_status = OKAY;
    }

    return water_pump_status;
}

// TESTS
void setUp(void) {}
void tearDown(void) {}

// Test functions for water valve
void reset_water_level_variables(void)
{
    TEST_ASSERT_TRUE(set_test_water_level_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_water_level_sensor_status());

    TEST_ASSERT_TRUE(set_test_water_level(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_water_level());

    TEST_ASSERT_TRUE(set_test_water_level_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_water_level_target_min());

    TEST_ASSERT_TRUE(set_test_water_level_target_max(90));
    TEST_ASSERT_EQUAL_UINT8(90, get_water_level_target_max());
}

void test_water_valve_uninitialized(void)
{
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_valve_status());
}

void test_water_level_target_values(void)
{
    watering_init();
    TEST_ASSERT_TRUE(set_test_water_level_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_water_level_sensor_status());
    TEST_ASSERT_TRUE(set_test_water_level(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_water_level());

    // target min is not valid
    TEST_ASSERT_FALSE(set_test_water_level_target_min(101));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_valve_status());

    // target max is not valid
    TEST_ASSERT_FALSE(set_test_water_level_target_max(101));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_valve_status());

    // target min and max are valid
    TEST_ASSERT_TRUE(set_test_water_level_target_min(10));
    TEST_ASSERT_TRUE(set_test_water_level_target_max(90));
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_valve_status());  // Subtest
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());
}

void test_water_level_sensor_error_safe_mode(void)
{
    // Normal system run
    watering_init();
    reset_water_level_variables();

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());


    // water level sensor error occurs
    TEST_ASSERT_TRUE(set_test_water_level_sensor_status(ERROR));
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_water_level_sensor_status());

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
}

void test_water_valve_is_overwritten(void)
{
    // Normal system run
    watering_init();
    reset_water_level_variables();
    overwrite_test_water_valve_state(NO);

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());


    // Overwrite the water valve state and set the water valve to be "ON"
    overwrite_test_water_valve_state(YES);
    TEST_ASSERT_TRUE(set_test_water_valve_state(ON));

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_valve_state());


    // Overwrite the water valve state and set the water valve to be "OFF"
    overwrite_test_water_valve_state(YES);
    TEST_ASSERT_TRUE(set_test_water_valve_state(OFF));
    
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
}

void test_water_valve_logic(void)
{
    watering_init();
    reset_water_level_variables();
    overwrite_test_water_valve_state(NO);

    // Test water level above target max
    TEST_ASSERT_TRUE(set_test_water_level(95));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to decrease the water level to the same level as the target max
    TEST_ASSERT_TRUE(set_test_water_level(90));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to decrease the water level below the target max, but above the average of target min/max
    TEST_ASSERT_TRUE(set_test_water_level(88));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to decrease the water level to the same level as the average of target min/max
    TEST_ASSERT_TRUE(set_test_water_level(50));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to decrease the water level below the average of target min/max, but above target min
    TEST_ASSERT_TRUE(set_test_water_level(40));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to decrease the water level below target min
    TEST_ASSERT_TRUE(set_test_water_level(5));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to increase the water level above target min again, but below the average of target min/max
    TEST_ASSERT_TRUE(set_test_water_level(20));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to increase the water level to the same level as the average of target min/max
    TEST_ASSERT_TRUE(set_test_water_level(50));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());

    // Test to increase the water level above the average of target min/max, but below target max
    TEST_ASSERT_TRUE(set_test_water_level(51));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_valve_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_valve_status());
}

// Test functions for water pump
void reset_soil_moisture_variables(void)
{
    TEST_ASSERT_TRUE(set_test_soil_moisture_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_soil_moisture_sensor_status());

    TEST_ASSERT_TRUE(set_test_soil_moisture(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_soil_moisture());

    TEST_ASSERT_TRUE(set_test_soil_moisture_target_min(10));
    TEST_ASSERT_EQUAL_UINT8(10, get_soil_moisture_target_min());

    TEST_ASSERT_TRUE(set_test_soil_moisture_target_max(90));
    TEST_ASSERT_EQUAL_UINT8(90, get_soil_moisture_target_max());
}

void test_water_pump_uninitialized(void)
{
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_pump_status());
}

void test_soil_moisture_target_values(void)
{
    watering_init();
    TEST_ASSERT_TRUE(set_test_soil_moisture_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_soil_moisture_sensor_status());
    TEST_ASSERT_TRUE(set_test_soil_moisture(50));
    TEST_ASSERT_EQUAL_UINT8(50, get_soil_moisture());

    // target min is not valid
    TEST_ASSERT_FALSE(set_test_soil_moisture_target_min(101));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_pump_status());

    // target max is not valid
    TEST_ASSERT_FALSE(set_test_soil_moisture_target_max(101));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_pump_status());

    // target min and max are valid
    TEST_ASSERT_TRUE(set_test_soil_moisture_target_min(10));
    TEST_ASSERT_TRUE(set_test_soil_moisture_target_max(90));
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, get_test_water_pump_status());  // Subtest
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
}

void test_soil_moisture_sensor_error_safe_mode(void)
{
    // Normal system run
    watering_init();
    reset_soil_moisture_variables();

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());


    // soil moisture sensor error occurs
    TEST_ASSERT_TRUE(set_test_soil_moisture_sensor_status(ERROR));
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_soil_moisture_sensor_status());

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
}

void test_water_pump_is_overwritten(void)
{
    // Normal system run
    watering_init();
    reset_soil_moisture_variables();
    overwrite_test_water_pump_state(NO);

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());


    // Overwrite the water pump state and set the water pump to be "ON"
    overwrite_test_water_pump_state(YES);
    TEST_ASSERT_TRUE(set_test_water_pump_state(ON));

    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_pump_state());


    // Overwrite the water pump state and set the water pump to be "OFF"
    overwrite_test_water_pump_state(YES);
    TEST_ASSERT_TRUE(set_test_water_pump_state(OFF));
    
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
}

void test_water_pump_logic(void)
{
    watering_init();
    reset_soil_moisture_variables();
    overwrite_test_water_pump_state(NO);

    // Test soil moisture level above target max
    TEST_ASSERT_TRUE(set_test_soil_moisture(95));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to decrease the soil moisture level to the same level as the target max
    TEST_ASSERT_TRUE(set_test_soil_moisture(90));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to decrease the soil moisture level below the target max, but above the average of target min/max
    TEST_ASSERT_TRUE(set_test_soil_moisture(88));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to decrease the soil moisture level to the same level as the average of target min/max
    TEST_ASSERT_TRUE(set_test_soil_moisture(50));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to decrease the soil moisture level below the average of target min/max, but above target min
    TEST_ASSERT_TRUE(set_test_soil_moisture(40));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to decrease the soil_moisture level below target min
    TEST_ASSERT_TRUE(set_test_soil_moisture(5));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to increase the soil moisture level above target min again, but below the average of target min/max
    TEST_ASSERT_TRUE(set_test_soil_moisture(20));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to increase the soil moisture level to the same level as the average of target min/max
    TEST_ASSERT_TRUE(set_test_soil_moisture(50));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());

    // Test to increase the soil moisture level above the average of target min/max, but below target max
    TEST_ASSERT_TRUE(set_test_soil_moisture(51));
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
}

void test_check_water_consumed(void)
{
    watering_init();
    reset_soil_moisture_variables();
    overwrite_test_water_pump_state(NO);
    TEST_ASSERT_TRUE(set_test_soil_moisture(5));
    TEST_ASSERT_EQUAL_UINT8(5, get_soil_moisture());
    TEST_ASSERT_TRUE(set_test_flow_meter_sensor_status(OKAY));
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_flow_meter_sensor_status());
    TEST_ASSERT_TRUE(set_test_flow_rate(200));
    TEST_ASSERT_EQUAL_UINT8(200, get_flow_rate());

    int run;
    // It requires 100 runs to get "water_consumed" = "MAX_WATER_PER_RUN"
    for(run = 1; run < 100; run++)
    {
        #ifdef DEBUG
        printf("Run no: %d\n", run);
        #endif

        watering_run();

        #ifdef DEBUG
        printf("Pump state no: %d\n", get_test_water_pump_state());
        #endif

        TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_pump_state());
        TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
    }

    // The pump will sleep in 10 seconds, which corresponds to 100 runs
    for(run = 100; run < 200; run++)
    {
        watering_run();
        // After the first run "water_consumed" should be equal to "MAX_WATER_PER_RUN"
        // and the pump will go to sleep
        TEST_ASSERT_EQUAL_UINT8(OFF, get_test_water_pump_state());
        TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
    }

    // The pump should now wake up
    watering_run();
    TEST_ASSERT_EQUAL_UINT8(ON, get_test_water_pump_state());
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_test_water_pump_status());
}

#ifdef TARGET

void loop() {}

void setup()
{
    delay(3000);
#else

int main(void)
{
#endif
    UNITY_BEGIN();

    // Water valve
    RUN_TEST(test_water_valve_uninitialized);
    RUN_TEST(test_water_level_target_values);
    RUN_TEST(test_water_level_sensor_error_safe_mode);
    RUN_TEST(test_water_valve_is_overwritten);
    RUN_TEST(test_water_valve_logic);

    // Water pump
    RUN_TEST(test_water_pump_uninitialized);
    RUN_TEST(test_soil_moisture_target_values);
    RUN_TEST(test_soil_moisture_sensor_error_safe_mode);
    RUN_TEST(test_water_pump_is_overwritten);
    RUN_TEST(test_water_pump_logic);
    RUN_TEST(test_check_water_consumed);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}