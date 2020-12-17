#include <common.h>
#include <unity.h>
#include <lamp.h>
#include <bsp.h>
#include <bsp_io.h>

#define SCALE (2.55f)

static int16_t state;
static int16_t temp;
static lamp_interface_t *interface = NULL; // A pointer to the interface. If NULL, the normal digitalWrite and pinMode will be used

#ifdef TARGET

void analogWrite_spy(uint8_t pin, uint8_t val)
{
    if (val >= ON)
    {
        state = val;
        bsp_analog_write(pin, val);
    }
    else
    {
        state = val;
        bsp_analog_write(pin, val);
    }
}

void pinMode_spy(uint8_t pin, uint8_t mode)
{
    bsp_pin_mode(pin, mode);
}

#else

void bsp_analog_write(uint8_t pin, uint8_t val)
{
    state = (val >= 1 && val <= 255) ? val : 0;
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
}
#endif

void setUp()
{
}

void tearDown()
{
}

void test_lamp_state_init(void)
{
    lamp_init(interface);
    lamp_set_state(100);
    temp = 100 * SCALE;
    TEST_ASSERT_UINT8_WITHIN(15, temp, state);

    lamp_set_state(0);
    temp = 0 * SCALE;
    TEST_ASSERT_UINT8_WITHIN(15, temp, state);
}

void test_lamp_state_not_init(void)
{
    lamp_set_state(50);
    TEST_ASSERT_EQUAL_UINT8(0U, state);

    lamp_set_state(100);
    TEST_ASSERT_EQUAL_UINT8(0U, state);

    lamp_set_state(0);
    TEST_ASSERT_EQUAL_UINT8(0U, state);
}

void test_set_lamp_state_on(void)
{
    lamp_init(interface);
    TEST_ASSERT_EQUAL_UINT8(1U, lamp_set_state(10));  // 10% light
    TEST_ASSERT_EQUAL_UINT8(1U, lamp_set_state(70));  // 70% light
    TEST_ASSERT_EQUAL_UINT8(1U, lamp_set_state(100)); // 100% light
}

void test_set_lamp_state_close(void)
{
    lamp_init(interface);
    TEST_ASSERT_EQUAL_UINT8(1U, lamp_set_state(0));   // 0% light / off
    TEST_ASSERT_EQUAL_UINT8(2U, lamp_set_state(-25)); // ivalid value. Lamp will be off.
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000U);

    lamp_interface_t spy_intrf = {};
    spy_intrf.bsp_pin_mode = pinMode_spy;
    spy_intrf.bsp_analog_write = analogWrite_spy;
    interface = &spy_intrf;

#else

int main(void)
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_lamp_state_not_init);
    RUN_TEST(test_lamp_state_init);
    RUN_TEST(test_set_lamp_state_on);
    RUN_TEST(test_set_lamp_state_close);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}