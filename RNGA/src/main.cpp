#include <RNGA.h>
#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }

    rnga_init();
}

void loop()
{
    rnga_seed(micros());
    Serial.println(rnga_generate(10, 100));
    delay(rnga_generate(100, 500));
}