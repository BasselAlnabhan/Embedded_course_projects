/**
 * @file eeprom_driver.cpp
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-11-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <EEPROM.h>
#include <common.h>
#include <eeprom_driver.h>
#include <Arduino.h>

static uint16_t space = 0;
static uint16_t used_space = 0;

uint8_t eeprom_driver_read(uint16_t address, uint8_t *data, uint8_t length)
{
    uint8_t status = ERROR;
    if (length && (address + length <= used_space))
    {
        for (uint8_t i = 0; i < length; i++)
        {
            data[i] = EEPROM.read(address + sizeof(used_space) + i);
            // Serial.printf("Ads R %d \n", address + sizeof(used_space) + i);
            // Serial.printf("data R %d \n", data[i]);
        }

        status = OKAY;
    }
    return status;
}

uint8_t eeprom_driver_write(uint16_t address, uint8_t *data, uint8_t length)
{
    uint8_t status = ERROR;
    if (length && (address + sizeof(used_space) + length < space))
    {
        for (uint8_t i = 0; i < length; i++)
        {
            EEPROM.write(address + sizeof(used_space) + i, data[i]);
            // Serial.printf("Ads W %d \n", address + sizeof(used_space) + i);
            // Serial.printf("data W %d \n", data[i]);
        }

        if (address + length > used_space)
        {
            used_space += length;
            EEPROM.put(0, used_space);
        }
        status = OKAY;
    }
    return status;
}

void eeprom_driver_init(bool reset)
{
    space = EEPROM.length();
    if (reset)
    {
        EEPROM.put(0, used_space); // The word is used to store the used bytes
    }
    else
    {
        EEPROM.get(0, used_space); // The word is used to store the used bytes
    }
}
