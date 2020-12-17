#include <common.h>
#include <unity.h>
#include <water_valve.h>
#include <bsp.h>
#include <bsp_io.h>

static volatile int16_t state;
static valve_interface_t *interface = NULL; // A pointer to the interface. If NULL, the normal digitalWrite and pinMode will be used

#ifdef TARGET

void digitalWrite_spy(uint8_t pin, uint8_t val)
{
    if (val == ON)
    {
        state = ON;
        bsp_digital_write(pin, val);
    }
    else
    {
        state = OFF;
        bsp_digital_write(pin, val);
    }
}

uint8_t digitalRead_spy(uint8_t pin)
{
    return bsp_digital_read(pin);
}

void pinMode_spy(uint8_t pin, uint8_t mode)
{
    bsp_pin_mode(pin, mode);
}

#else

void bsp_digital_write(uint8_t pin, uint8_t val)
{
    state = (val == 1) ? 1 : 0;
}

uint8_t bsp_digital_read(uint8_t pin)
{
    return state;
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

void test_water_valve_init_ok(void)
{
    water_valve_init(NULL);
    TEST_ASSERT_EQUAL_UINT8(1U, water_valve_set_state_off());

    water_valve_init(interface);
    TEST_ASSERT_EQUAL_UINT8(1U, water_valve_set_state_on());
}

void test_water_valve_init_not_ok(void)
{
    TEST_ASSERT_EQUAL_UINT8(0U, water_valve_set_state_off());

    TEST_ASSERT_EQUAL_UINT8(0U, water_valve_set_state_on());
}

void test_set_water_valve_state_open(void)
{
    TEST_ASSERT_EQUAL_UINT8(1U, water_valve_set_state_on());
}

void test_set_water_valve_state_close(void)
{
    TEST_ASSERT_EQUAL_UINT8(1U, water_valve_set_state_off());
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    valve_interface_t spy_intrf = {};
    spy_intrf.pin_mode = pinMode_spy;
    spy_intrf.digital_write = digitalWrite_spy;
    spy_intrf.digital_read = digitalRead_spy;
    interface = &spy_intrf;

    delay(3000U);
#else

int main(void)
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_water_valve_init_not_ok);
    RUN_TEST(test_water_valve_init_ok);
    RUN_TEST(test_set_water_valve_state_open);
    RUN_TEST(test_set_water_valve_state_close);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}
