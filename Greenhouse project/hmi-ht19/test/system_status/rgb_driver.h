/*@author Adrian M*/

#ifndef RGB_DRIVER_H
#define RGB_DRIVER_H

#include <stdint.h>

typedef struct
{
    void (*digital_write)(uint8_t, uint8_t);
    void (*pin_mode)(uint8_t, uint8_t);
} interface_t;

/**
 * @brief This function is used to initialize the module, It will Set the active the default led color to (yellow), 
 *        the led state to (yellow) and the led status to (OKAY).
 * @param intrfc An interface pointer. If this parameter is NULL, the default functions will be used.
 */
void rgb_driver_init(interface_t *intrfc);

/**
 * @brief This function will turn on the green light,
 * and set the led state to GREEN 
 * @return uint8_t (UNINIT or OKAY).
 */
uint8_t rgb_driver_green_light(void);

/**
 * @brief This function will turn on the red light, 
 * and set the led state to RED. 
 * @return uint8_t (UNINIT or OKAY).
 */
uint8_t rgb_driver_red_light(void);

/**
 * @brief This function will turn on the yrllow light.
 * and set the led state to YELLOW. 
 * @return uint8_t (UNINIT or OKAY).
 */
uint8_t rgb_driver_yellow_light(void);

#endif /* RGB_DRIVER_H */
