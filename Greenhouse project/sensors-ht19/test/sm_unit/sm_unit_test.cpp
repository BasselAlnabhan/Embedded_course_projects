#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <common.h>
#include "canbus.h"
#include <sm.h>

static uint8_t soil_moisture;        // Return stub variable for moisture
static uint8_t canbus_sensor_status; // Varible holding the status of the sensor set on canbus
static uint8_t canbus_soil_moisture; // Variable holding the soil moisture reading set on canbus
static bool driver_init_called;      // Variable holding if driver init function has been called

void reset_variables()
{
    soil_moisture = 0xFFU;
    canbus_sensor_status = 0xFFU;
    canbus_soil_moisture = 0xFFU;
    driver_init_called = false;
}

static void set_stub_soil_moisture(uint8_t soil_stub)
{
    soil_moisture = soil_stub;
}

uint8_t soil_moisture_driver_read(uint8_t *moisture)
{
    *moisture = soil_moisture;

    return OKAY;
}

void soil_moisture_driver_init(void)
{
    driver_init_called = true;
}

void set_soil_moisture(uint8_t value)
{
    canbus_soil_moisture = value;
}
void set_soil_moisture_sensor_status(uint8_t value)
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

void test_sm(void)
{
    // Test if module is uninitialized if init function has not been called
    sm_app_update();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(0, canbus_soil_moisture);
    TEST_ASSERT_FALSE(driver_init_called);

    // Normal use case
    reset_variables();
    set_stub_soil_moisture(50);
    sm_app_init();
    sm_app_update();
    TEST_ASSERT_TRUE(driver_init_called);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(50, canbus_soil_moisture);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000);
#else
int main()
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_sm);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}