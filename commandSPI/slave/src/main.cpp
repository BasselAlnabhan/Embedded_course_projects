/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief In this slave program, the built-in LED on Teensy is controlled via SPI.
 * @version 0.1
 * @date 2020-05-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <TSPISlave.h>

#define ON '1'
#define OFF '0'
#define SPI_SCK 14

TSPISlave slave = TSPISlave(SPI, PIN_SPI_MISO, PIN_SPI_MOSI, SPI_SCK, PIN_SPI_SS);

/**
 * @brief This function is called if there is a received data from the master
 */
void receive(void)
{
    while (slave.active())
    {
        if (slave.available())
        {
            uint8_t data = slave.popr(); // Read the received data
            if ((data == OFF) || (data == ON))
            {
                digitalWrite(LED_BUILTIN, (data == ON) ? HIGH : LOW);
                slave.pushr(digitalRead(LED_BUILTIN) ? ON : OFF); // Send the status of the LED to the master
            }
        }
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    slave.onReceive(receive);
}

void loop()
{
}
