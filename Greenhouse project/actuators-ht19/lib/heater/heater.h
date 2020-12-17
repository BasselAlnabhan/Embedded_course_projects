/**
 * @file heater.h
 * @author Valentine 
 * @brief Ambient Heater in a Smart Greenhouse Project 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef HEATER_H_
#define HEATER_H_

#include <stdint.h>

#define HEATER_PIN (20U) /** @brief The heater module will be connected digital pin 6 */

typedef struct
{
    void (*analog_write)(uint8_t, uint8_t);
    void (*pin_mode)(uint8_t, uint8_t);
} heater_interface_t;

/**
 * @brief Function for the initialization of the heater module
 * @param heater_interface; A pointer to the heater interface
 */
void heater_init(heater_interface_t *heater_interface);

/**
* @brief Function assigned a new state to the heater, and  return the state
*        which could be ON, OFF or ERROR
* @param value The PWM value  
* @return uint8_t return OKAY, ERROR or UNINITIALIZED
*/
uint8_t heater_set_state(uint8_t value);

#endif /* HEATER_H_ */
