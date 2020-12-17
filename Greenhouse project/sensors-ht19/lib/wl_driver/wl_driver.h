/**
 * @file water_level_driver.h
 * @author Alain (AlainDlcTVOP)
 * @brief Driver of the water level sensor.
 * @version 0.1
 * @date 2020-10-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WL_DRIVER_H
#define WL_DRIVER_H

#include <stdint.h>

#define WATER_LEVEL_PIN (33)

/**
 * @brief function to initialized the driver
 * @remark has to be run first before sensor can be read
 */
void driver_water_level_init(void);

/**
 * @brief to read the resistance of the sensor
 * 
 * @param length  Address to the output for the read value, if ERROR occurs length will be 0;
 * @return uint8_t status of sensor, OKAY if successful else an ERROR
 */
uint8_t water_level_driver_read(uint8_t *length);

#endif /* WL_DRIVER_H */