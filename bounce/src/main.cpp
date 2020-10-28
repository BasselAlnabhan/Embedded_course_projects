#include <Metro.h>
#include <Arduino.h>

#define DURATION 100
#define BUTTON_PIN 33

uint32_t count = 0;
uint32_t previousCount = 0;
uint8_t previousState = HIGH;
Metro timer = Metro(DURATION);

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(9600);
    delay(2000);
}

void loop()
{
    uint8_t currentState = digitalRead(BUTTON_PIN);

    if (currentState != previousState)
    {
        if (currentState == LOW)
        {
            count++;
        }
        previousState = currentState;
    }
    else if ((count != previousCount) && timer.check())
    {
        Serial.printf("count: %d\n", count);
        previousCount = count;
    }
}
