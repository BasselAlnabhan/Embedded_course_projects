/*

       ---------
      |  App    | - Integration test
      |         |
      |  Driver |     CAN - fake
       ---------                  
            bsp - fake

*/

#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <bsp_io.h>
#include <ldr.h>
#include <common.h>
#include "canbus.h"

static uint8_t bits_value;             // Variable holding the bit value set on "bsp_analog_read_resolution()".
static uint16_t bsp_analog_read_value; // Return stub variable of "bsp_analog_read()".
static uint8_t canbus_ldr_value;       // Variable holding the ldr value set on canbus.
static uint8_t canbus_status;          // Variable holding the status of the sensor set on canbus.

static void reset_variables()
{
    bits_value = 0XFF;             // Initialize with a value outside its boundaries (2 - 32).
    bsp_analog_read_value = 1234U; // Initialize with a value outside its boundaries (for 10 bits: 0 - 1023).
    canbus_ldr_value = 0xFFU;      // Initialize with a value outside its boundaries (0 - 100%).
    canbus_status = 0xFF;          // Initialize with a value not taken in one of the "status-makros" ("common.h").
}

void bsp_analog_read_resolution(uint8_t bits)
{
    bits_value = bits;
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return bsp_analog_read_value;
}

void set_light_intensity(uint8_t value)
{
    canbus_ldr_value = value;
}

void set_light_intensity_sensor_status(uint8_t value)
{
    canbus_status = value;
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

void test_ldr_app()
{
    // Test when the module is uninitialized
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(0, canbus_ldr_value);
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_status);

    // Test the module with initialization
    reset_variables();
    ldr_app_init();
    TEST_ASSERT_EQUAL_UINT8(10, bits_value);

    // Test normal use of case, check if status = OKAY
    reset_variables();

    ldr_app_init();
    TEST_ASSERT_EQUAL_UINT8(10, bits_value);
    bsp_analog_read_value = 200U;
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(80, canbus_ldr_value);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_status);

    // Check if status = OKAY
    reset_variables();
    bsp_analog_read_value = 9U;
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_status);

    // Check if status = OKAY
    reset_variables();
    bsp_analog_read_value = 994U;
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_status);

    // Check if status = ERROR
    reset_variables();
    bsp_analog_read_value = 8U;
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(ERROR, canbus_status);

    // Check if status = ERROR
    reset_variables();
    bsp_analog_read_value = 995U;
    ldr_app_update();
    TEST_ASSERT_EQUAL_UINT8(ERROR, canbus_status);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000); // We need to have a delay because the tempory file
                 // that is being created in a "TEENSY test" run doesn't include a delay
                 // after "Serial.begin()".
#else
int main()
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_ldr_app);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}