#ifdef TARGET
#include <bsp.h>
#endif

#include <bsp_io.h>
#include <unity.h>
#include <rgb_driver.h>
#include <common.h>

static int16_t state;                 // This variable is used to test if the number of written values to the pins are correct and in order
static uint8_t outpins;               // Number of pins in OUTPUT mode
static uint16_t n_times;              // Number of digital writes
static interface_t *interface = NULL; // A pointer to the interface. NULL means that we will use the default functions which are pinMode, delay and digitalWrite

static void reset_variables(void)
{
    state = 0;
    outpins = 0;
    n_times = 0;
}

#ifdef TARGET
void digital_write_spy(uint8_t pin, uint8_t val)
{
    n_times++; // Number of calls to digitalWrite
    if (val == HIGH)
    {
        state++; // If we set the pin to HIGH, state is increamented
    }
    else
    {
        state--; // If we set the pin to LOW, state is decreamented
    }
    bsp_digital_write(pin, val); // bsp_digital_write on Teensy
}

void pin_mode_spy(uint8_t pin, uint8_t mode)
{
    outpins++;               // If the mode is correct, increament outpins
    bsp_pin_mode(pin, mode); // bsp_pin_mode on Teensy
}



#else
void bsp_digital_write(uint8_t pin, uint8_t val)
{
    n_times++;
    if (val == HIGH)
    {
        state++;
    }
    else
    {
        state--;
    }
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    outpins++;
}

void bsp_tone()
{

}

#endif

void setUp()
{
    reset_variables();
}

void tearDown()
{
}

void test_rgb_drirver_init()
{
    /* Test if the module has been initialized*/
    rgb_driver_init(interface);
    TEST_ASSERT_EQUAL_UINT8(outpins, 2);
    TEST_ASSERT_EQUAL_INT16(2, state);
}

void test_rgb_driver_green_light()
{
    /* Test The Green Light */
    rgb_driver_init(interface);
    TEST_ASSERT_EQUAL_UINT8(OKAY, rgb_driver_green_light());
    TEST_ASSERT_EQUAL_UINT8(outpins, 2);
    TEST_ASSERT_EQUAL_INT16(1, state);
    TEST_ASSERT_EQUAL_INT16(5, n_times);
}

void test_rgb_driver_red_light()
{
    /* Test The Red Light */
    rgb_driver_init(interface);
    TEST_ASSERT_EQUAL_UINT8(OKAY, rgb_driver_red_light());
    TEST_ASSERT_EQUAL_UINT8(outpins, 2);
    TEST_ASSERT_EQUAL_INT16(1, state);
    TEST_ASSERT_EQUAL_INT16(5, n_times);
}

void test_rgb_driver_yellow_light()
{
    /* Test The Yellow Light */
    rgb_driver_init(interface);
    TEST_ASSERT_EQUAL_UINT8(OKAY, rgb_driver_yellow_light());
    TEST_ASSERT_EQUAL_UINT8(outpins, 2);
    TEST_ASSERT_EQUAL_INT16(2, state);
    TEST_ASSERT_EQUAL_INT16(6, n_times);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    /**
     * @brief On Teensy we provide our implementation for digitalWrite and pinMode functions
     *        and we assign these functions to the function pointers of the interface
     */
    interface_t temp = {};
    temp.digital_write = digital_write_spy;
    temp.pin_mode = pin_mode_spy;
    interface = &temp;
    delay(3000U);

#else
int main(void)
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_rgb_drirver_init);
    RUN_TEST(test_rgb_driver_green_light);
    RUN_TEST(test_rgb_driver_red_light);
    RUN_TEST(test_rgb_driver_yellow_light);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}