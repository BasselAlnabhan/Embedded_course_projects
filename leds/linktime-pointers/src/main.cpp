#include <bsp.h>
#include <blinky.h>

void setup()
{
    Serial.begin(9600);
    bsp_delay(3000);

    uint8_t leds[] = {1, 2, 3, 13};

    if (BLINKY_ERROR == blinky_begin(NULL, leds, sizeof(leds)))
    {
        Serial.println("Failed to initialize the blinky module!");
    }

    if (BLINKY_ERROR == blinky_blink_times(5, 200))
    {
        Serial.println("Failed to blink 5 times!");
    }

    if (BLINKY_ERROR == blinky_blink_times(10, 500))
    {
        Serial.println("Failed to blink 10 times!");
    }

    blinky_end();
}

void loop()
{
}
