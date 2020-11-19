#include <ioport.h>
#include <kinetis.h>

#define ALL_HIGH ((uint32_t)0xFF)
#define ALL_OUTPUT ((uint32_t)0xFF)
#define PIN_MUX_GPIO ((uint32_t)1 << 8U)

/**
 * Configure 8 pins of PORTD as GPIO outputs
 */
void io_port_init(void)
{
    // Set the pins as GPIO
    PORTD_PCR0 = PIN_MUX_GPIO; // Pin 2
    PORTD_PCR1 = PIN_MUX_GPIO; // Pin 14
    PORTD_PCR2 = PIN_MUX_GPIO; // Pin 7
    PORTD_PCR3 = PIN_MUX_GPIO; // Pin 8
    PORTD_PCR4 = PIN_MUX_GPIO; // Pin 6
    PORTD_PCR5 = PIN_MUX_GPIO; // Pin 20
    PORTD_PCR6 = PIN_MUX_GPIO; // Pin 21
    PORTD_PCR7 = PIN_MUX_GPIO; // Pin 5

    // Set the pins as outputs
    GPIOD_PDDR = ALL_OUTPUT;

    // Set the pins to HIGH
    GPIOD_PDOR = ALL_HIGH;
}

volatile uint8_t *io_port_address(void)
{
    return (volatile uint8_t *)(&GPIOD_PDOR);
}
