/**
 * @file air_fan.h
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief An interface to use the Air-Fan
 * @version 0.1
 * @date 2020-10-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef AIR_FAN_DRIVER_H_
#define AIR_FAN_DRIVER_H_

#include <stdint.h>

#define FAN_PIN (1U) /** @brief Means that pin should be a digital pin*/

typedef struct
{
    void (*digital_write)(uint8_t, uint8_t);
    void (*pin_mode)(uint8_t, uint8_t);
    uint8_t (*digital_read)(uint8_t);
} fan_interfc_t;

/**
 * @brief  Used to initialize the fan actuator
 * @param interface A pointer to the implemented air fan interface used
 */
void air_fan_init(fan_interfc_t *interface);

/**
 * @brief Used to turn on the air fan actuator
 * @return uint8_t ,as the status of the actuator
 */
uint8_t air_fan_turn_on(void);

/**
 * @brief Used to turn off the air fan actuator
 * @return uint8_t ,as the status of the actuator
 */
uint8_t air_fan_turn_off(void);

#endif /* AIR_FAN_DRIVER_H_ */