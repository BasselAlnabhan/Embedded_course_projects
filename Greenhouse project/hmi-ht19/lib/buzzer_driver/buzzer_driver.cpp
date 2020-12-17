/**
 * @file buzzer_driver.cpp
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-11-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <bsp.h>
#include <bsp_io.h>
#include <buzzer_driver.h>

#define PIN_BUZZER 23 //PIN_A9

void buzzer_driver_init(void)
{
    bsp_pin_mode(PIN_BUZZER, OUTPUT);
}

void buzzer_driver_tone(uint16_t frequency, uint32_t duration)
{
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(PIN_BUZZER, frequency, duration);
}

void buzzer_driver_no_tone(void)
{
    // stop the waveform generation before the next note.
    noTone(PIN_BUZZER);
}