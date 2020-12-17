/**
 * @file water_pump.h
 * @author Valentine (you@domain.com)
 * @brief Water_pump interface
 * @version 0.1
 * @date 2020-11-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef WATER_PUMP_H_
#define WATER_PUMP_H_

#include <stdint.h>

#define WATER_PUMP_PIN (23U) /** @brief The pump will be connected to an analog pin*/

typedef struct
{
    void (*pin_mode)(uint8_t pin, uint8_t mode);
    void (*digital_write)(uint8_t pin, uint8_t val);
    uint8_t (*digital_read)(uint8_t pin);
} water_pump_interface_t;

/**
 * @brief  Used to initialize the water_pump actuator
 * @param interface A pointer to the implemented water_pump interface used
 */
void water_pump_init(water_pump_interface_t *interface);

/**
 * @brief Used to turn on the water_pump actuator
 * @return uint8_t ,as the status of the actuator
 */
uint8_t water_pump_turn_on(void);

/**
 * @brief this Function is used to turn off the water_pump actuator
 * @return the status of the water_pump
 */
uint8_t water_pump_turn_off(void);

#endif /* WATER_PUMP_H_ */