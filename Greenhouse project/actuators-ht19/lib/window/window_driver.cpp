/**
 * @file window_driver.cpp
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief A special implementation to the Window ventilator to be used in the greenhouse project
 * @version 0.1
 * @date 2020-11-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <Servo.h>
#include <common.h>
#include <window_driver.h>

#define CLOSED_DEGREES (90U)
#define OPEN_DEGREES (0U)

static Servo window_motor;

uint8_t window_driver_init()
{
    window_motor.attach(WINDOW_PIN);
    return (window_motor.attached()) ? OKAY : UNINITIALIZED;
}

uint8_t window_driver_open()
{
    uint8_t status = UNINITIALIZED;

    if (window_motor.attached())
    {
        window_motor.write(OPEN_DEGREES);
        status = OKAY;
    }

    return status;
}

uint8_t window_driver_close()
{
    uint8_t status = UNINITIALIZED;

    if (window_motor.attached())
    {
        window_motor.write(CLOSED_DEGREES);
        status = OKAY;
    }

    return status;
}
