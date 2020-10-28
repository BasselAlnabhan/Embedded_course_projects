#include <Metro.h>
#include <Bounce.h>
#include <Arduino.h>

#define BUTTON_PIN 33
#define DURATION 100
#define INTERVAL 10

uint32_t count = 0;
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
    if (pushbutton.update())
    {
        if (pushbutton.fallingEdge())
        {
            count++;
        }
    }
    else if ((count != previousCount) && timer.check())
    {
        Serial.printf("count: %d\n", count);
        previousCount = count;
    }
}