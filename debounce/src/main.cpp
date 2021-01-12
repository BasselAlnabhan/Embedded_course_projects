#include <Metro.h>
#include <Bounce.h>
#include <Arduino.h>

#define BUTTON_PIN 33
#define DURATION 100
#define INTERVAL 10

uint32_t interval = 1U;
uint32_t countFalling = 0;
uint32_t countRising = 0;
uint32_t previousCount = 0;
Metro timer = Metro(DURATION);
Bounce pushbutton = Bounce(BUTTON_PIN, INTERVAL);

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(9600);
    delay(2000);
}

void loop()
{
    Bounce buttonX = Bounce(BUTTON_PIN, interval);
    if (timer.check())
    {
        if (buttonX.update())
        {
            if (buttonX.fallingEdge())
            {
                countFalling++;
                Serial.printf("button falling is detected %d times\n", countFalling);
            }
            else if (buttonX.risingEdge())
            {
                countRising++;
                Serial.printf("button rising is detected %d times\n", countRising);
            }
        }
    }
}