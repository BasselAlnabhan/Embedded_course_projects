/**
 * @file sm_driver.h
 * @author Mohammed Fazlur Rahman (raseleee02@gmail.com)
 * @brief Soil moisture driver
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SOIL_MOISTURE_DRIVER_H
#define SOIL_MOISTURE_DRIVER_H

#include <stdint.h>

/**
 * @brief Function to initialized the driver
 * @remark Function has to be run first before sensor can be read
 */
void soil_moisture_driver_init(void);

/**
 * @brief Function to read the soil moisture sensor
 * 
 * @param moisture Address to the output for the sensor in % value 0-100
 * 
 * @return status of the sensor, OKAY if successful else ERROR
 */
uint8_t soil_moisture_driver_read(uint8_t *moisture);

#endif /* SOIL_MOISTER_DRIVER_H */
