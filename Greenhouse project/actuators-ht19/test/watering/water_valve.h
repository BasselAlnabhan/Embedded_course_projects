/**
 * @file water_valve.h
 * @author Daniel Sandblom
 * @brief The water valve driver
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef WATER_VALVE_H
#define WATER_VALVE_H

#include <stdint.h>

#define VALVE_PIN (10U)

typedef struct
{
    void (*pin_mode)(uint8_t pin, uint8_t mode);
    void (*digital_write)(uint8_t pin, uint8_t val);
    uint8_t (*digital_read)(uint8_t pin);
} valve_interface_t;

/**
 * @brief This function is used for initialization of module
 * 
 * @param interface A pointer to the interface that holds the function pointers
 *                  used for fake digitalWrite and pinMode if TEENSY not included.
 *                  If TEENSY included, assign them to the spy version of the functions.
 */
void water_valve_init(valve_interface_t *interface);

/**
 * @brief This function is used for setting the state of the water valve on.
 * 
 * @return the status of the water valve.
 */
uint8_t water_valve_set_state_on(void);

/**
 * @brief This function is used for setting the state of the water valve off.
 * 
 * @return the status of the water valve.
 */
uint8_t water_valve_set_state_off(void);

#endif /* WATER_VALVE_H */