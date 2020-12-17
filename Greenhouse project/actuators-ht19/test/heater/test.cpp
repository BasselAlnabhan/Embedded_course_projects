#include <unity.h>
#include <bsp_io.h>
#include <heater.h>
#include <common.h>

#ifdef TARGET
#include <Arduino.h>
#endif

#define NO_MODE (20U)
#define SCALE (2.55f)

static heater_interface_t *heater_interface = NULL;
static uint8_t mode_spy;
static uint8_t status;
static uint8_t state;

#ifdef TARGET
void bsp_pin_mode_spy(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}
void bsp_analog_write_spy(uint8_t pin, uint8_t val)
{
    state = (uint8_t)(SCALE * val);
    analogWrite(pin, state);
}
#else
void bsp_analog_write(uint8_t pin, uint8_t val)
{
    if (val == HIGH)
    {
        state = ON;
    }
    else
    {
        state = OFF;
    }
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    mode_spy = mode;
}
#endif

void variables_reset(void)
{
    state = 0;
    status = 0;
    mode_spy = NO_MODE;
}

void setUp(void)
{
    variables_reset();
}

void tearDown(void)
{
}

void test_heater_init_null_interface(void)
{
    heater_init(NULL);
    TEST_ASSERT_EQUAL_UINT8(OKAY, heater_set_state(100));
}

void test_set_state_on(void)
{
    heater_init(heater_interface);
    TEST_ASSERT_EQUAL_UINT8(OKAY, heater_set_state(99));
}

void test_set_state_off(void)
{
    heater_init(heater_interface);
    TEST_ASSERT_EQUAL_UINT8(OKAY, heater_set_state(0));
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    Serial.begin(9600);
    delay(2000);

    heater_interface_t temp = {};
    temp.analog_write = bsp_analog_write_spy;
    temp.pin_mode = bsp_pin_mode_spy;
    heater_interface = &temp;

#else

int main(void)
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_heater_init_null_interface);
    RUN_TEST(test_set_state_on);
    RUN_TEST(test_set_state_off);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}