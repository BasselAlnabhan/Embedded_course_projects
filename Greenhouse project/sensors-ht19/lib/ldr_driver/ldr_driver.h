/**
 * @file ldr_driver.h
 * @author Stefan Edenholm (stefan.edenholm@yaelev.se)
 * @brief Driver for the light intensity sensor
 * @version 0.1
 * @date 2020-11-12
 * 
 * @copyright Copyright (c) 2020
 * 
 *              ---------------------
 *              |   UNINITIALIZED   | read()
 *              ---------------------
 *                |                  
 *         init() |               
 *                |                
 *                V               
 *            ------------ NOT NULL           -------------
 *   read()   |    OK    | <----------------> |   ERROR   | read()
 *   NOT NULL ------------       read()       ------------- NULL, outside calibration interval
 *                                       NULL
 *                            outside calibration interval
 */

#ifndef LDR_DRIVER_H
#define LDR_DRIVER_H

#include <stdint.h>

/**
 * @brief Initializing the light intensity sensor.
 * 
 * @remark Has to be run first.
 */
void ldr_driver_init(void);

/**
 * @brief Read the light intensity from the sensor and convert it to a percent value.
 * 
 * @param ldr_value A pointer used for reading the ldr value, which is a percent value between 0 and 100.
 * @return The status value which can be "UNINITIALIZED", "OKAY" or "ERROR".
 */
uint8_t ldr_driver_read(uint8_t *ldr_value);

#endif /* LDR_DRIVER_H */