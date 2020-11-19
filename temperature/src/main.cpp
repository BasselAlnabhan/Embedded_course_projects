#include <Arduino.h>

#define ADC_RESOLUTION 10U                                       // The resolution of the ADC. It can be 8, 10, 12 or 16. By default it is 10 bits
#define VOLTAGE_OFFSET 500U                                      // According to the TMP36 sensor we have 0.5 volt (500 millivolts) offset
#define MILLIVOLTS_PER_CELSIUS 10U                               // According to the TMP36 sensor every 10 millivolts is 1 grad celsius
#define VOLTAGE_RESOLUTION (1000 * 3.3f / (1 << ADC_RESOLUTION)) // The voltage resolution of the ADC in millivolts

void setup()
{
    // Initial and begin the USB serial communication
    Serial.begin(9600);

    // Wait until the serial communication gets initialized and started
    while (!Serial)
    {
    }

    // Set the ADC resolution
    analogReadResolution(ADC_RESOLUTION);
}

void loop()
{
    /**
    * Wait 1 second (1000 millisecond).
    * The sensor is read every second
   **/
    delay(1000);

    // Read the data from the analog pin and convert it to millivolts
    float milliVolts = analogRead(A14) * VOLTAGE_RESOLUTION;

    // Convert the voltage to temperature according to the datasheet of TMP36
    float temperature = (milliVolts - VOLTAGE_OFFSET) / MILLIVOLTS_PER_CELSIUS;

    // Print the temperature to the terminal
    Serial.printf("Temperature: %.2f\n", temperature);
}