#ifndef BSP_IO_H
#define BSP_IO_H

#include <stdint.h>
#ifdef TARGET
#include <Servo.h>
#endif

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

/**
 * @brief This function is used to set a mode to a pin
 * @param pin The pin number to be set
 * @param mode The mode to be set to the pin
 */
void bsp_pin_mode(uint8_t pin, uint8_t mode);

/**
 * @brief This function is used to write a HIGH/LOW to a specific pin
 * @param pin Is the pin number to write to
 * @param value HIGH/LOW to be written to the pin
 */
void bsp_digital_write(uint8_t pin, uint8_t value);
/**
 * @brief This function is used to attach the motor pin
 * @param pin the pin number for the motor
 */
void bsp_servo_attach(uint8_t pin);
/**
 * @brief Tis function is used to check if the pin is attached to the motor
 * @return true if the pin is attached
 * @return false otherwise
 */
bool bsp_servo_attached();
/**
 * @brief This function is used to write an angle to the motor
 */
void bsp_servo_write(uint8_t angle);
#endif
/* BSP_IO_H */