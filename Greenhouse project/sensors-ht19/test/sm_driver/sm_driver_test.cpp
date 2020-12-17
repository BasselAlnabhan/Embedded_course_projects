#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <bsp_io.h>
#include <sm_driver.h>
#include <common.h>

static bool bits_set;        // Variable to check if bits has been set
static uint8_t moisture;     // Variable for storing sensor output value
static uint16_t return_stub; // Variable to set stub value of bsp_analog_read()

void reset_variables()
{
    return_stub = 0U;
    bits_set = false;
    moisture = 0U;
}

void bsp_analog_read_resolution(uint8_t bits)
{
    if (bits == 10)
    {
        bits_set = true;
    }
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return return_stub;
}

void setUp()
{
    reset_variables();
}

void tearDown()
{
}

void test_soil_moisture_driver(void)
{
    // Test if module is uninitialized if no call to soil_moisture_driver_init() has been done
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, soil_moisture_driver_read(&moisture));

    // Normal use case, check if status of sensor ha been set to OKAY
    reset_variables();
    soil_moisture_driver_init();
    return_stub = 500U;
    TEST_ASSERT_EQUAL_UINT8(OKAY, soil_moisture_driver_read(&moisture));

    // Check if error occur when NULL pointer is passed to function
    reset_variables();
    soil_moisture_driver_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, soil_moisture_driver_read(NULL));

    reset_variables();
    // Above calibrated dry value
    return_stub = 902U;
    TEST_ASSERT_EQUAL_UINT8(ERROR, soil_moisture_driver_read(&moisture));

    // below calibrated wet value
    return_stub = 359U;
    TEST_ASSERT_EQUAL_UINT8(ERROR, soil_moisture_driver_read(&moisture));

    // Check if resultion is set to 10 bits
    reset_variables();
    soil_moisture_driver_init();
    TEST_ASSERT_TRUE(bits_set);
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
    RUN_TEST(test_soil_moisture_driver);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}
