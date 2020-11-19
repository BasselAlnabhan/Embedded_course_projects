#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <temperature.h>

TMP36::TMP36(int a, float b)
{
  analogPin = a;
  voltage = b;
}

void TMP36::update()
{
  int reading = analogRead(analogPin);

  // converting that reading to voltage
  volts = reading * (voltage / 1024);

  // converting from 10 mv per degree wit 500 mV offset
  // to degrees ((voltage - 500mV) times 100)
  celsius = (volts - 0.5) * 100;

  // convert celsius to kelvin
  kelvin = celsius + 273.15;

  // convert celsius to fahrenheit
  fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
}

float TMP36::getVolts()
{
  return volts;
}

float TMP36::getKelvin()
{
  return kelvin;
}

float TMP36::getCelsius()
{
  return celsius;
}

float TMP36::getFahrenheit()
{
  return fahrenheit;
}
