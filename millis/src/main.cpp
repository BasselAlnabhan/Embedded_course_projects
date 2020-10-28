#include <Arduino.h>

uint16_t counter = 0U;
uint8_t ledState = LOW;
uint32_t lastMillis = 0U;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600U);
    delay(2000U);
}

void loop()
{
    uint32_t now = millis();

    if (lastMillis > now) // When the timer get overflowed
    {
        counter += (0xFFFFFFFFU - lastMillis) + 1U + now;
    }
    else
    {
        counter += (now - lastMillis);
    }

    lastMillis = now;

    if (counter >= 500U)
    {
        counter = 0U;
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }
}