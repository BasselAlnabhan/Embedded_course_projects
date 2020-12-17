#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <bsp_io.h>
#include <sm.h>
#include <common.h>
#include "canbus.h"

static uint8_t bits_value;             // Variable holding the bit value set on "bsp_analog_read_resolution()".
static uint16_t bsp_analog_read_value; // Return stub variable of "bsp_analog_read()".
static uint8_t canbus_sensor_status;   // Varible holding the status of the sensor set on canbus
static uint8_t canbus_soil_moisture;   // Variable holding the soil moisture reading set on canbus

static void reset_variables()
{
    bits_value = 0xFF;             // Initialize with a value outside its boundaries (2 - 32).
    bsp_analog_read_value = 1234U; // Initialize with a value outside its boundaries (for 10 bits: 0 - 1023).
    canbus_soil_moisture = 0xFFU;  // Initialize with a value outside its boundaries (0 - 100%).
    canbus_sensor_status = 0xFFU;  // Initialize with a value not taken in one of the "status-makros" ("common.h").
}

void bsp_analog_read_resolution(uint8_t bits)
{
    bits_value = bits;
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return bsp_analog_read_value;
}

void set_soil_moisture(uint8_t value)
{
    canbus_soil_moisture = value;
}
void set_soil_moisture_sensor_status(uint8_t value)
{
    canbus_sensor_status = value;
}

// Running before every "RUN_TEST(...)"

void setUp()
{
    reset_variables();
}

// Running after every "RUN_TEST(...)"
void tearDown()
{
}

void test_sm_app_integration()
{
    // Test when the module is uninitialized
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(0, canbus_soil_moisture);
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_sensor_status);

    // Test the module with initialization
    reset_variables();
    sm_app_init();
    TEST_ASSERT_EQUAL_UINT8(10, bits_value);

    // Test normal use of case, check if status = OKAY
    reset_variables();
    sm_app_init();
    bsp_analog_read_value = 500U;
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(74, canbus_soil_moisture);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);

    // Check if status = OKAJ
    reset_variables();
    bsp_analog_read_value = 360U;
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);

    // Check if status = OKAJ
    reset_variables();
    bsp_analog_read_value = 901U;
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);

    // Check if status = ERROR
    reset_variables();
    bsp_analog_read_value = 359U;
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(ERROR, canbus_sensor_status);

    // Check if status = ERROR
    reset_variables();
    bsp_analog_read_value = 902U;
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(ERROR, canbus_sensor_status);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000); // We need to have a delay because the tempory file
                 // that is being created in a test run doesn't include a delay.
#else
int main()
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_sm_app_integration);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}