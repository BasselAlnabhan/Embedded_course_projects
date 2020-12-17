/**
 * @file wl_driver.h
 * @author Alain (alain_dlc@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WL_DRIVER_H
#define WL_DRIVER_H

#include <stdint.h>

/**
 * @brief function to initialized the driver
 * @remark has to be run first before sensor can be read
 */
void driver_water_level_init(void);

/**
 * @brief to read the resistance of the sensor
 * 
 * 
 * @param length  Address to the output for the read value, if ERROR occurs length will be 0;
 * @return uint8_t status of sensor, OKAY if successful else an ERROR
 */
uint8_t water_level_driver_read(uint8_t *length);

#endif /* WL_DRIVER_H */