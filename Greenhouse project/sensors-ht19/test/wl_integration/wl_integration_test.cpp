
#ifdef TARGET
#include <Arduino.h>
#endif
#include <unity.h>
#include <common.h>
#include "canbus.h"
#include <bsp_io.h>
#include <wl.h>

static uint8_t length;     // Variable for storing sensor resistance
static uint16_t read_stub; // Variable to set stub value of bsp_analog_read()
static bool bit_set;       // Variable to check if bits has been set
static uint8_t water_level;
static uint8_t canbus_sensor_status; // Varible holding the status of the sensor set on canbus
static uint8_t canbus_water_level;   // Variable holding the water_level reading set on canbus
static bool driver_init_called;      // Variable holding if driver init function has been called

void reset_variables()
{
    bit_set = false;
    read_stub = 0U;
    length = 0U;
    water_level = 0xFFU;

    canbus_sensor_status = 0xFFU;
    canbus_water_level = 0xFFU;
    driver_init_called = false;
}

void set_water_level(uint8_t value)
{
    canbus_water_level = value;
}
void set_water_level_sensor_status(uint8_t value)
{
    canbus_sensor_status = value;
}

void bsp_analog_read_resolution(uint8_t bits)
{
    if (bits == 10)
    {
        bit_set = true;
    }
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return read_stub;
}
void setUp()
{
    reset_variables();
}

void tearDown()
{
}

void test_water_level_app()
{
    // Test when the module is uninitialized
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(0, canbus_water_level);

    // Normal use case
    reset_variables();
    read_stub = 400U;
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(54, canbus_water_level);

    // Test percentage less than MAX_PERCENTAGE
    reset_variables();
    read_stub = 290U;
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(81, canbus_water_level);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);

    // Test percentage more than MAX_PERCENTAGE
    reset_variables();
    read_stub = 171U;
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(ERROR, canbus_sensor_status);

    // Test min water level sensor 1500 ohm
    reset_variables();
    read_stub = 512U;
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);

    // Test min water level sensor 1505 ohm
    reset_variables();
    read_stub = 513U;
    water_level_app_init();
    water_level_app_update();
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

    RUN_TEST(test_water_level_app);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}
