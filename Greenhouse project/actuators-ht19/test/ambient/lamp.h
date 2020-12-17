/**
 * @file lamp.h
 * @author Daniel Sandblom
 * @brief The LED lamp driver
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef LAMP_H
#define LAMP_H

#include <stdint.h>

#define LAMP_PIN (10U)

typedef struct
{
    void (*bsp_analog_write)(uint8_t, uint8_t);
    void (*bsp_pin_mode)(uint8_t, uint8_t);
    uint8_t (*bsp_analog_read)(uint8_t pin);
} lamp_interface_t;

/**
 * @brief This function is used for initialization of module
 * 
 * @param interface A pointer to the interface that holds the function pointers
 *                  used for fake analogWrite and pinMode if TEENSY not included.
 *                  If TEENSY included, assign them to the spy version of the functions.
 */
void lamp_init(lamp_interface_t *interface);

/**
 * @brief This function is used to write a PWM value to the lamp
 * 
 * @param value The PWM value we write to the lamp.
 * @return uint8_t return OKAY, ERROR or UNINITIALIZED
 */
uint8_t lamp_set_state(uint8_t value);

#endif /* LAMP_H */