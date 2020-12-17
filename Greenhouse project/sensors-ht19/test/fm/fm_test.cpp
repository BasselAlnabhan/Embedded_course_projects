#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <common.h>
#include "canbus.h"
#include <fm.h>

static uint16_t flow_rate;           // Return stub variable for flow_rate
static uint8_t canbus_sensor_status; // Varible holding the status of the sensor set on canbus
static uint16_t canbus_flow_rate;    // Variable holding the flow_rate reading set on canbus
static bool driver_init_called;      // Variable holding if driver init function has been called

void reset_variables()
{
    flow_rate = 0xFFFFU;

    canbus_sensor_status = 0xFF;
    canbus_flow_rate = 0xFFFF;
    driver_init_called = false;
}

static void set_stub_flow_rate(uint16_t flow_stub)
{
    flow_rate = flow_stub;
}

uint8_t flow_meter_driver_read(uint16_t *flow)
{
    *flow = flow_rate;

    return OKAY;
}

void flow_meter_driver_init(void)
{
    driver_init_called = true;
}

void set_flow_rate(uint16_t value)
{
    canbus_flow_rate = value;
}
void set_flow_meter_sensor_status(uint8_t value)
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

void test_fm_app(void)
{
    // Test if module is uninitialized if init function has not been called
    fm_app_update();
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(0, canbus_flow_rate);
    TEST_ASSERT_FALSE(driver_init_called);

    // Normal use case
    reset_variables();
    set_stub_flow_rate(200);
    fm_app_init();
    fm_app_update();
    TEST_ASSERT_TRUE(driver_init_called);
    TEST_ASSERT_EQUAL_UINT8(OKAY, canbus_sensor_status);
    TEST_ASSERT_EQUAL_UINT8(200, canbus_flow_rate);
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
    RUN_TEST(test_fm_app);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}