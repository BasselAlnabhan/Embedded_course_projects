#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

/**
 * @brief Sets the size (in bits) of the value returned by Arduino's "analogRead()".
 *        For example 10 bits returns values from "analogRead()" between 0 - 1023.
 * 
 * @param bits The bit value.
 */
void bsp_analog_read_resolution(uint8_t bits);

/**
 * @brief Reads the value from the specified analog pin, given a bit resolution.
 * 
 * @param pin The number of the analog input pin to read from.
 * @return The analog reading on the pin.
 */
uint16_t bsp_analog_read(uint8_t pin);

#endif