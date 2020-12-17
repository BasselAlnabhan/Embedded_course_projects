/**
 * @file buzzer_driver.h
 * @author Adrian Morkvist
 * @brief driver for the Buzzer on teensy
 * @version 0.1
 * @date 2020-11-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef BUZZER_DRIVER_H
#define BUZZER_DRIVER_H

#include <stdint.h>

/**
 * @brief init function
 */
void buzzer_driver_init(void);

/**
 * @brief function to play a note.
 * 
 * @param frequency the pitch of the note
 * @param duration how long the note should be played 
 */
void buzzer_driver_tone(uint16_t frequency, uint32_t duration);

/**
 * @brief making a pause in the sound output
 * 
 */
void buzzer_driver_no_tone(void);

#endif /* BUZZER_DRIVER_H */
