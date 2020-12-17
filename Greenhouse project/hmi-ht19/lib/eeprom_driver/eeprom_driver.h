/**
 * @file eeprom_driver.h
 * @author Adrian Morkvist
 * @brief Driver for the EEPROM on teensy
 * @version 0.1
 * @date 2020-11-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief init function, only really used here if you want to clear the stored eeprom data
 * 
 * @param reset, true if you want to clear the eeprom, false and this function does nothing.
 */
void eeprom_driver_init(bool reset);

/**
 * @brief Function to put(store) a complete struct in the eeprom
 * 
 * @param address pointer for where to start storing the data
 * @param val the structure to store
 * @return uint8_t returns the status, OKAY if successful, and ERROR if we have reached the end of the eeprom.
 */
uint8_t eeprom_driver_write(uint16_t address, uint8_t *data, uint8_t length);

/**
 * @brief Function to read stored data in the eeprom
 * 
 * @param address a pointer to where to start reading from
 * @return data_type_t returns the structure stored in the eeprom
 */
uint8_t eeprom_driver_read(uint16_t address, uint8_t *data, uint8_t length);

#endif /* EEPROM_DRIVER_H */