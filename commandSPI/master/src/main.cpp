/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief In this master program, the user input will be send to the slave via SPI in order to control the state of its built-in LED.
 * @version 0.1
 * @date 2020-05-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <SPI.h>
#include <Arduino.h>

#define ON '1'
#define OFF '0'
#define PIN_SPI_SS 33
#define CLOCK_RATE 1000000 // The speed of the communication

static bool show_message = true;
static SPISettings setting = SPISettings(CLOCK_RATE, MSBFIRST, SPI_MODE0); // The setting for the SPI communication

void setup()
{
    pinMode(PIN_SPI_SS, OUTPUT);
    digitalWrite(PIN_SPI_SS, HIGH);

    Serial.begin(9600);
    SPI.begin();
    delay(2000);
}

void loop()
{
    if (show_message)
    {
        show_message = false;
        Serial.print("Enter the command [0: Off, 1: On]: ");
    }

    if (Serial.available())
    {
        char chr = Serial.read();
        if ((chr == OFF) || (chr == ON))
        {
            Serial.print(chr);

            SPI.beginTransaction(setting); // Start the SPI transaction
            digitalWrite(PIN_SPI_SS, LOW); // Activate the chip select pin

            SPI.transfer(chr); // Transfer the state to the slave

            SPI.transfer(0);                 // Dummy byte
            uint8_t state = SPI.transfer(0); // Read the state of the built-in LED of the slave

            digitalWrite(PIN_SPI_SS, HIGH); // Deactivate the chip select
            SPI.endTransaction();           // End the SPI transaction

            Serial.println((state == chr) ? " => done" : " => failed");
            show_message = true;
            Serial.println(chr);
            Serial.println(state);
        }
        else if (chr == '\n')
        {
            show_message = true;
            Serial.println();
        }
    }
}
