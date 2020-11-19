#include <i2c_t3.h>

#define ADDRESS 0x35
#define CLOCK_RATE 400000 // In slave mode the clock rate is not used.

void requestEvent(void)
{
}

void receiveEvent(size_t length)
{
}

void setup()
{
    Wire.begin(I2C_SLAVE, ADDRESS, I2C_PINS_18_19, I2C_PULLUP_INT, CLOCK_RATE);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
}