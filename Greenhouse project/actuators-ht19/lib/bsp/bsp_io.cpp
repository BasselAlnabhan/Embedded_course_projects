#include <bsp.h>
#include <bsp_io.h>

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void bsp_digital_write(uint8_t pin, uint8_t value)
{
    digitalWrite(pin, value);
}

uint8_t bsp_digital_read(uint8_t pin)
{
    return digitalRead(pin);
}

uint8_t bsp_analog_read(uint8_t pin)
{
    return (uint8_t)analogRead(pin);
}

void bsp_analog_write(uint8_t pin, uint8_t val)
{
    analogWrite(pin, val);
}