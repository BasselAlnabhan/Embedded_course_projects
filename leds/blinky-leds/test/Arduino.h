/**
 * @file Arduino.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief This is the header file of the test double. 
 *        The functions have been implemented in the test file.
 *        The test double is used on the development enviroment.
 * @version 0.1
 * @date 2020-10-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef ARDUINO_H
#define ARDUINO_H

#define LOW 0
#define HIGH 1
#define OUTPUT 1

#include <stdint.h>

void delay(uint32_t ms);
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);

#endif /* ARDUINO_H */