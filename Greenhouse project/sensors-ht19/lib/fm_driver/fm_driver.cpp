/**
 * @file fm_driver.cpp
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief Implementation of flow meter driver
 * @version 0.1
 * @date 2020-10-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <fm_driver.h>
#include <stdlib.h>
#include <bsp_io.h>
#include <common.h>

#define FM_PIN 20
#define VOLUME_PER_PULSE ((float)1000 / (float)450)          // From datasheet 450 pulses per liter
#define OVERFLOW_MULTIPLIER (uint8_t)(VOLUME_PER_PULSE + 1U) // Multiplier to for overflow protection

volatile uint16_t pulse_frequency;
static uint8_t status = UNINITIALIZED;

void rpm(void)
{
    bsp_no_interrupts();
    pulse_frequency++;
    bsp_interrupts();
}

static bool check_overflow(void)
{
    bool overflow = false;
    uint16_t result = pulse_frequency * OVERFLOW_MULTIPLIER;
    if (pulse_frequency != 0 && (result / pulse_frequency) != OVERFLOW_MULTIPLIER)
    {
        overflow = true;
    }
    return overflow;
}

void flow_meter_driver_init(void)
{
    pulse_frequency = 0U;

    bsp_pin_mode(FM_PIN, INPUT_PULLUP);
    bsp_attach_interrupt(FM_PIN, rpm, RISING);
    bsp_interrupts();

    status = OKAY;
}

uint8_t flow_meter_driver_read(uint16_t *flow)
{
    // Check if the driver has been initialized
    if (status != UNINITIALIZED)
    {
        if (flow == NULL)
        {
            status = ERROR;
        }
        // Check if overflow error has occured
        else if (check_overflow())
        {
            // If an overflow occurs reset the counter
            bsp_no_interrupts();
            pulse_frequency = 0U;
            bsp_interrupts();
            status = ERROR;
        }
        else
        {
            // Calculate the total volume since last call
            *flow = pulse_frequency * ((float)VOLUME_PER_PULSE);
            status = OKAY;

            // Reset the counter for each call of the function
            bsp_no_interrupts();
            pulse_frequency = 0U;
            bsp_interrupts();
        }
    }
    return status;
}
