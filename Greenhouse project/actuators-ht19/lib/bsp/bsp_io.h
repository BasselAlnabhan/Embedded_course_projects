#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

/**
 * @brief This function is used to set a mode to a pin
 * @param pin The pin number to be set
 * @param mode The mode to be set to the pin
 */
void bsp_pin_mode(uint8_t pin, uint8_t mode);

/**
 * @brief This function is used to write to an analog pin
 * @param pin The pin number to write to
 * @param val The value to write
 */
void bsp_analog_write(uint8_t pin, uint8_t val);

/**
 * @brief This function is used to read from an anlog pin
 * 
 * @param pin The pin number
 * @return uint8_t The read value
 */
uint8_t bsp_analog_read(uint8_t pin);

/**
 * @brief This function is used read form a digital pin
 * 
 * @param pin The pin number
 * @return uint8_t The read value; HIGH/LOW
 */
uint8_t bsp_digital_read(uint8_t pin);

/**
 * @brief This function is used to write a HIGH/LOW to a specific pin
 * @param pin Is the pin number to write to
 * @param value HIGH/LOW to be written to the pin
 */
void bsp_digital_write(uint8_t pin, uint8_t value);

#endif /* BSP_IO_H */