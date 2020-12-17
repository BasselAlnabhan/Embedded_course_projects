/**
 * @file keypad_driver.h
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

// INCLUDES
#include <stdint.h>

typedef enum
{
    KEY_IDLE,
    KEY_PRESSED,
    KEY_HOLD,
    KEY_RELEASED
} key_state_t;

//FUNCTION DECLARATIONS

/**
 * @brief initialization of the keypad
 * 
 * @return uint8_t status, we return that the initialization has been done.
 */
void keypad_driver_init(void);

/**
 * @brief Function for returning whatever key we pressed on the keypad
 * 
 * @return char key, the key that we pressed.
 */
char keypad_driver_get_key(void);

/**
 * @brief Function for sharing the state of the keypad
 * 
 * @return KeyState kstate, the state of the keypad
 */
key_state_t keypad_driver_get_state(void);

#endif /* KEYPAD_DRIVER_H */
