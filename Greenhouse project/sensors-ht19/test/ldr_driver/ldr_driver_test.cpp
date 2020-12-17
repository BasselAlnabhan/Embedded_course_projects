#ifdef TARGET
#include <Arduino.h> // In link-time we need to use Arduino on Teensy
#endif

#include <unity.h>
#include <bsp_io.h>
#include <ldr_driver.h>
#include <common.h>

#define ADC_RESOLUTION (10U)

static uint8_t bits_value;      // Variable for holding the bit value.
static uint8_t ldr_value;       // Variable for storing ldr read value.
static uint16_t return_stub;    // Variable to set the stub value of "bsp_analog_read()".

static void reset_variables()
{
    bits_value = 0U;
    ldr_value = 0U;
    return_stub = 0U;
}

void bsp_analog_read_resolution(uint8_t bits)
{
    bits_value = bits;
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return return_stub;
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

void test_ldr_driver_read()
{
    // Test if the module is uninitialized
    TEST_ASSERT_EQUAL_UINT8(UNINITIALIZED, ldr_driver_read(&ldr_value));

    // Test the module with initialization
    reset_variables();
    ldr_driver_init();
    return_stub = 200U;
    TEST_ASSERT_EQUAL_UINT8(bits_value, ADC_RESOLUTION);
    TEST_ASSERT_EQUAL_UINT8(OKAY, ldr_driver_read(&ldr_value));
    TEST_ASSERT_EQUAL_UINT8(80, ldr_value);

    // Check if "ERROR" occurs when NULL pointer is passed to function
    reset_variables();
    ldr_driver_init();
    TEST_ASSERT_EQUAL_UINT8(ERROR, ldr_driver_read(NULL));

    // Test if equal to "CALIBRATION_LIGHT"
    reset_variables();
    return_stub = 9;
    TEST_ASSERT_EQUAL_UINT8(OKAY, ldr_driver_read(&ldr_value));

    // Test if equal to "CALIBRATION_DARK"
    reset_variables();
    return_stub = 994;
    TEST_ASSERT_EQUAL_UINT8(OKAY, ldr_driver_read(&ldr_value));

    // Test if lower than "CALIBRATION_LIGHT"
    reset_variables();
    return_stub = 8;
    TEST_ASSERT_EQUAL_UINT8(ERROR, ldr_driver_read(&ldr_value));

    // Test if higher than "CALIBRATION_DARK"
    reset_variables();
    return_stub = 995;
    TEST_ASSERT_EQUAL_UINT8(ERROR, ldr_driver_read(&ldr_value));
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000);    // You must have a delay because the tempory file
                    // that creates in a test run doesn't include delay.
#else
int main()
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_ldr_driver_read);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}