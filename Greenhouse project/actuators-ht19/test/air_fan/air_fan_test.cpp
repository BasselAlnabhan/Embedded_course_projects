/**
 * @file air_fan_test.cpp
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "bsp.h"
#include <unity.h>
#include <air_fan_driver.h>

#ifdef TARGET
#include <Arduino.h>
#endif

static fan_interfc_t *fan = NULL;
static uint8_t state;

#ifdef TARGET
void bsp_pin_mode_spy(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}
void bsp_digital_write_spy(uint8_t pin, uint8_t val)
{
    state = val;
    digitalWrite(pin, val);
}
uint8_t bsp_digital_read_spy(uint8_t pin)
{
    state = digitalRead(pin);
    return state;
}
#else
void bsp_digital_write(uint8_t pin, uint8_t val)
{
    if (val == 1)
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
}

uint8_t bsp_digital_read(uint8_t pin)
{
    return state;
}
#endif

void setUp(void)
{
}

void tearDown(void) {}

void test_air_fan_init_null_interface(void)
{
    air_fan_init(NULL);
    TEST_ASSERT_EQUAL_UINT8(OKAY, air_fan_turn_off());
}
void test_set_state(void)
{
    TEST_ASSERT_EQUAL_UINT8(OKAY, air_fan_turn_on());
}

void test_air_fan_init_interface(void)
{
    air_fan_init(fan);
    air_fan_turn_on();
    TEST_ASSERT_EQUAL_UINT8(ON, state);
}
void test_set_state2(void)
{
    air_fan_turn_off();
    TEST_ASSERT_EQUAL_UINT8(OFF, state);
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(3000);
    fan_interfc_t temp = {};
    temp.digital_write = bsp_digital_write_spy;
    temp.pin_mode = bsp_pin_mode_spy;
    temp.digital_read = bsp_digital_read_spy;
    fan = &temp;
#else

int main(void)
{
    fan_interfc_t temp = {};
    temp.digital_write = bsp_digital_write;
    temp.pin_mode = bsp_pin_mode;
    temp.digital_read = bsp_digital_read;
    fan = &temp;

#endif
    UNITY_BEGIN();
    RUN_TEST(test_air_fan_init_null_interface);
    RUN_TEST(test_set_state);
    RUN_TEST(test_air_fan_init_interface);
    RUN_TEST(test_set_state2);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}