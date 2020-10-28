#include <bsp.h>
#include <Arduino.h>

void bsp_digital_write(uint8_t pin, uint8_t val)
{
    digitalWrite(pin, val);
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void bsp_delay(uint32_t msec)
{
    delay(msec);
}