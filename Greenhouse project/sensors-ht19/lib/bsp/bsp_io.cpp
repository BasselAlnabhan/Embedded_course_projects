#include <bsp.h>
#include <bsp_io.h>

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void bsp_analog_read_resolution(uint8_t bits)
{
    analogReadResolution(bits);
}

uint16_t bsp_analog_read(uint8_t pin)
{
    return (uint16_t)analogRead(pin);
}

void bsp_attach_interrupt(uint8_t pin, void (*function)(void), int mode)
{
    attachInterrupt(pin, function, mode);
}

void bsp_interrupts(void)
{
    interrupts();
}

void bsp_no_interrupts(void)
{
    noInterrupts();
}
