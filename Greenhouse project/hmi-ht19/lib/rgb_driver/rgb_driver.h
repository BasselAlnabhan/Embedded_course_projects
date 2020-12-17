/**
 * @file rgb.h
 * @author Wael Al Henawe (wael.henawe@gmail.com)
 * @brief This modular is for initialize an instance of a RGB Led (240*230 2.0 TFT ST7789) and give the ability 
 *        to Light the led with three different colors (Green, Yellow, Red).
 * @version 0.1
 * @date 2020-10-27
 * 
 * @copyright Copyright (c) 2020
 */

#ifndef RGB_H
#define RGB_H

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

#endif /* RGB_H */