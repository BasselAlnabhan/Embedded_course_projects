#include <Arduino.h>

#define DELAY 50
#define MAX_VOLTAGE 3.0f
#define DAC_RESOLUTION 12
#define REFERENCE_VOLTAGE 3.3f
#define VOLTAGE_STEP (MAX_VOLTAGE / (1000 / DELAY))
#define VOLTAGE_RESOLUTION (REFERENCE_VOLTAGE / (1 << DAC_RESOLUTION))

void setup()
{
    analogWriteResolution(DAC_RESOLUTION);
}

void loop()
{
    float voltage = 0.0f;

    while (voltage <= MAX_VOLTAGE)
    {
        delay(DELAY);
        analogWriteDAC0((int)(voltage / VOLTAGE_RESOLUTION));
        voltage += VOLTAGE_STEP;
    }

    while (voltage >= 0)
    {
        delay(DELAY);
        analogWriteDAC0((int)(voltage / VOLTAGE_RESOLUTION));
        voltage -= VOLTAGE_STEP;
    }
}