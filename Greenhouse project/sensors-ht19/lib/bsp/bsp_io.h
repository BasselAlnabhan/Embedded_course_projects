#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef INPUT
#define INPUT 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif

#ifndef RISING
#define RISING 3
#endif

/**
 * @brief Function to set the mode of the pin
 * 
 * @param pin The pin number to set the mode of.
 * @param mode The pin mode (INPUT or OUTPUT).
 */
void bsp_pin_mode(uint8_t pin, uint8_t mode);

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

/**
 * @brief Function to attach interrupt
 * 
 * @param pin the pin the interrupt should be attatched
 * @param function The interrupt function
 * @param mode the interurupt mode
 */
void bsp_attach_interrupt(uint8_t pin, void (*function)(void), int mode);

/**
 * @brief Function to disable interrupts
 * 
 */
void bsp_no_interrupts(void);

/**
 * @brief Function to enable interrupts
 * 
 */
void bsp_interrupts(void);

#endif
