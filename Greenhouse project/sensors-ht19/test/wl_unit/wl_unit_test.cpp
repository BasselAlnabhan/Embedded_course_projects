
#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <common.h>
#include "canbus.h"
#include <wl.h>

static uint8_t water_level;
static uint8_t canbus_sensor_status; // Varible holding the status of the sensor set on canbus
static uint8_t canbus_water_level;   // Variable holding the water_level reading set on canbus
static bool driver_init_called;      // Variable holding if driver init function has been called

void reset_variables()
{
    water_level = 0xFFU;
    canbus_sensor_status = 0xFFU;
    canbus_water_level = 0xFFU;
    driver_init_called = false;
}

static void set_stub_water_level(uint8_t water_level_stub)
{
    water_level = water_level_stub;
}

void driver_water_level_init(void)
{
    driver_init_called = true;
}
uint8_t water_level_driver_read(uint8_t *length)
{
    *length = water_level;

    return OKAY;
}

void set_water_level(uint8_t value)
{
    canbus_water_level = value;
}
void set_water_level_sensor_status(uint8_t value)
{
    canbus_sensor_status = value;
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

    // Test if module is uninitialized if init function has not been called
    water_level_app_update();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(0, canbus_water_level);
    TEST_ASSERT_FALSE(driver_init_called);

    // Normal use case both min and max calibration is valid

    reset_variables();
    set_stub_water_level(10);
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_TRUE(driver_init_called);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(54, canbus_water_level);

    // Test percentage more than MAX_PERCENTAGE
    reset_variables();
    set_stub_water_level(17);
    water_level_app_init();
    water_level_app_update();
    TEST_ASSERT_TRUE(driver_init_called);
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
