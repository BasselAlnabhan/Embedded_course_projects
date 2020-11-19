#include <Arduino.h>

#define KILO 1000
#define ADC_RESOLUTION 10                                             // The resolution of the ADC.
#define SUPPLIED_VOLTAGE 3.3f                                         // The supplied voltage of the ADC
#define LDR_MIN_RESISTANCE (2 * KILO)                                 // The resistance of the LDR in 10 lux is something between 2 kΩ and 5kΩ
#define LDR_MAX_RESISTANCE (5 * KILO)                                 // The resistance of the LDR in 10 lux is something between 2 kΩ and 5kΩ
#define RESISTOR_RESISTANCE (10 * KILO)                               // The resistance of the resistor
#define VOLTAGE_RESOLUTION (SUPPLIED_VOLTAGE / (1 << ADC_RESOLUTION)) // The voltage resolution of the ADC

// An integer variable to store the state of the built in LED
uint8_t ledState = LOW;

void setup()
{
    // Initialize and begin the USB serial communication
    Serial.begin(9600);

    // Wait until the serial communication gets initialized and started
    delay(2000);

    // Set the mode of the LED pin to OUTPUT
    pinMode(LED_BUILTIN, OUTPUT);

    // Set the resolution of the ADC
    analogReadResolution(ADC_RESOLUTION);
}

void loop()
{
    // Read the data from the analog pin and convert it to voltage
    float ldrVoltage = analogRead(A14) * VOLTAGE_RESOLUTION;

    // Calculate the resistance of the LDR
    float ldrResistance = (RESISTOR_RESISTANCE * ldrVoltage) / (SUPPLIED_VOLTAGE - ldrVoltage);

    // Print the resistance of the LDR to the terminal
    Serial.printf("The resistance of the LDR is %.3f kΩ\n", ldrResistance / KILO);

    if (ldrResistance < LDR_MIN_RESISTANCE)
    {
        /**
     * If the resistance of the LDR is lesser than 2 kΩ,
     * it means that the light intensity is greater than 10 lux
     * and therefore the LED will blink
     */
        ledState = !ledState;
    }
    else if (ldrResistance > LDR_MAX_RESISTANCE)
    {
        /**
     * If the resistance of the LDR is greater than 5 kΩ,
     * it means that the light intensity is lesser than 10 lux
     * and therefore the LED will be turned on
     */
        ledState = HIGH;
    }
    else
    {
        /**
     * If the resistance of the LDR is between 2 kΩ and 5 kΩ,
     * it means that the light intensity is almost 10 lux
     * and therefore the LED will be turned off
     */
        ledState = LOW;
    }

    // Change the state of the LED
    digitalWrite(LED_BUILTIN, ledState);

    //Wait 500 milliseconds
    delay(500);
}
