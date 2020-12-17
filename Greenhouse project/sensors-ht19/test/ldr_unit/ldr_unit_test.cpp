/*

            App - Unit test

    Driver - fake       CAN - fake
    
            bsp - not used

*/

#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <ldr.h>
#include <common.h>
#include "canbus.h"

static bool ldr_driver_init_called;   // Variable holding if ldr_driver_init() has been called.
static uint8_t ldr_driver_read_value; // Return stub variable of "ldr_driver_read()".
static uint8_t canbus_ldr_value;      // Variable holding the ldr value set on canbus.
static uint8_t canbus_status;         // Variable holding the status of the sensor set on canbus.

static void reset_variables()
{
    ldr_driver_init_called = false;
    ldr_driver_read_value = 0xFFU; // Initialize with a value outside its boundaries (0 - 100%).
    canbus_ldr_value = 0xFFU;      // Initialize with a value outside its boundaries (0 - 100%).
    canbus_status = 0xFF;          // Initialize with a value not taken in one of the "status-makros" ("common.h").
}

void ldr_driver_init(void)
{
    ldr_driver_init_called = true;
}

uint8_t ldr_driver_read(uint8_t *ldr_value)
{
    *ldr_value = ldr_driver_read_value;
    return OKAY;
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
    TEST_ASSERT_FALSE(ldr_driver_init_called);

    // Normal use case
    reset_variables();
    ldr_driver_read_value = 50;
    ldr_app_init();
    ldr_app_update();
    TEST_ASSERT_TRUE(ldr_driver_init_called);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_status);
    TEST_ASSERT_EQUAL_UINT8(50, canbus_ldr_value);
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