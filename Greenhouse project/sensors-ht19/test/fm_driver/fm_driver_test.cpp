#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <bsp_io.h>
#include <fm_driver.h>
#include <common.h>

static bool interrupt_attatched; // Variable to check if interrupt has been attatched
static uint8_t status;           // Holds value for the status of the sensor
static uint16_t flow_value;      // Variable for storing sensor read value for fluid flow
static bool pullup;              // Variable to check if pullup mode has been set
static bool state_interupt;      // Variable to check if interupts remain enabled

void reset_variables()
{
    interrupt_attatched = false;
    status = 0xFFU;
    flow_value = 0U;
    pullup = false;
    state_interupt = false;
}

void bsp_interrupts(void)
{
    state_interupt = true;
#ifdef TARGET
    interrupts();
#endif
}
void bsp_no_interrupts(void)
{
    state_interupt = false;
#ifdef TARGET
    noInterrupts();
#endif
}

void bsp_attach_interrupt(uint8_t pin, void (*function)(void), int mode)
{
    interrupt_attatched = true;
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    if (mode == INPUT_PULLUP)
    {
        pullup = true; // check if set to pullup
    }
}

void setUp()
{
    reset_variables();
}

void tearDown()
{
}

void test_flow_meter_driver(void)
{
    //Test if module is uninitialized if no call to flow_meter_driver_init() has been done
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, flow_meter_driver_read(&flow_value));

    // Normal use case, check if status of sensor has been set to OKAY
    reset_variables();
    flow_meter_driver_init();
    TEST_ASSERT_EQUAL_UINT8(OKAY, flow_meter_driver_read(&flow_value));

    // Check if error occur when NULL pointer is passed to function
    reset_variables();
    flow_meter_driver_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, flow_meter_driver_read(NULL));

    //Test if flow_meter_driver_init() enables interupts
    reset_variables();
    flow_meter_driver_init();
    TEST_ASSERT_TRUE(pullup);
    TEST_ASSERT_TRUE(state_interupt);

    //Test if flow_meter_driver_read() keep interupts enabled after call
    reset_variables();
    flow_meter_driver_read(&flow_value);
    TEST_ASSERT_TRUE(state_interupt);

    //Test if flow_meter_driver_init() has attached interupt
    reset_variables();
    flow_meter_driver_init();
    TEST_ASSERT_TRUE(interrupt_attatched);
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
    RUN_TEST(test_flow_meter_driver);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}
