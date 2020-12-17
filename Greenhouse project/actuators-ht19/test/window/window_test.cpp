/**
 * @file window_test.cpp
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifdef TARGET
#include <Arduino.h>
#include <Servo.h>
Servo motor;
#endif

#include <unity.h>
#include <window_driver.h>
#include <bsp_io.h>
#include <bsp.h>

static uint8_t state;
static bool attached;

void setUp(void) {}

void tearDown(void) {}

void bsp_servo_attach(uint8_t pin)
{
    attached = true;

#ifdef TARGET
    motor.attach(WINDOW_PIN);
#endif
}

bool bsp_servo_attached()
{
    return attached;

#ifdef TARGET
    motor.attached();
#endif
}

void bsp_servo_write(uint8_t angle)
{
    if (angle == 0 || angle == 90)
    {
        state = angle;
        if (angle == 90)
            state = OPEN;
#ifdef TARGET
        motor.write(angle);
#endif
    }
}

void test_window_init(void)
{
    TEST_ASSERT_EQUAL_UINT8(1, window_driver_init());
}

void test_window_open(void)
{

    TEST_ASSERT_EQUAL_UINT8(1, window_driver_open());
    // TEST_ASSERT_EQUAL_UINT8(OPEN, state);
}
void test_window_closed(void)
{

    TEST_ASSERT_EQUAL_UINT8(1, window_driver_close());
    TEST_ASSERT_EQUAL_UINT8(CLOSE, state);
}
#ifdef TARGET
void loop()
{
}

void setup()
{
    delay(2000);
#else

int main(void)
{

#endif
    UNITY_BEGIN();
    RUN_TEST(test_window_init);
    RUN_TEST(test_window_open);
    RUN_TEST(test_window_closed);
#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}