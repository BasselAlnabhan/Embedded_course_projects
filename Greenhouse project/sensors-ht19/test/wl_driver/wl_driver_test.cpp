#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <bsp_io.h>
#include <wl_driver.h>
#include <common.h>

static uint8_t length;     // Variable for storing sensor resistance
static uint16_t read_stub; // Variable to set stub value of bsp_analog_read()
static bool bit_set;       // Variable to check if bits has been set

void reset_variables()
{
    bit_set = false;
    read_stub = 0U;
    length = 0U;
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

void test_water_level_driver(void)
{

    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, water_level_driver_read(&length));

    // Normal use case, check if status of sensor has been set to OKAY
    reset_variables();
    driver_water_level_init();
    read_stub = 410; // 1001 ohm
    TEST_ASSERT_EQUAL_UINT8(OKAY, water_level_driver_read(&length));

    // Check if error occur when NULL pointer is passed to function
    reset_variables();
    driver_water_level_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, water_level_driver_read(NULL));

    //Test if analog_resolution is set to 10bits;
    reset_variables();
    driver_water_level_init();
    TEST_ASSERT_TRUE(bit_set);

    // Test if error occur if outside acceptable min value (300 ohm)
    reset_variables();
    read_stub = 170; // 298 ohm
    TEST_ASSERT_EQUAL_UINT8(ERROR, water_level_driver_read(&length));

    // Test if error occur if outside acceptable max value (1500 ohm)
    reset_variables();
    read_stub = 513; // 1505 ohm
    TEST_ASSERT_EQUAL_UINT8(ERROR, water_level_driver_read(&length));

    // Test limits
    reset_variables();
    read_stub = 171; // 300 ohm
    TEST_ASSERT_EQUAL_UINT8(OKAY, water_level_driver_read(&length));

    // Test limits
    reset_variables();
    read_stub = 512; // 1500 ohm
    TEST_ASSERT_EQUAL_UINT8(OKAY, water_level_driver_read(&length));
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
    RUN_TEST(test_water_level_driver);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}