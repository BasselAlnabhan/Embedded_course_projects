/**
 * @file window_driver.h
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief An interface to use the Window ventilator
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef WINDOW_DRIVER_H
#define WINDOW_DRIVER_H

#include <stdint.h>

#define WINDOW_PIN (3) /** @brief Means that pin should be a digital pin*/

/**
 * @brief This function is used to initialize a window ventilator.
 * @return uint8_t As the status of the window.
 */
uint8_t window_driver_init(void);

/**
 * @brief This function is used to open window.
 * @return uint8_t As the status of the window.
 */
uint8_t window_driver_open(void);

/**
 * @brief This function is used to close window.
 * @return uint8_t As the status of the window.
 */
uint8_t window_driver_close(void);

#endif /* WINDOW_DRIVER_H */