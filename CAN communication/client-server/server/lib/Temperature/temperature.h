// TMP36 Pin Variables
// the analog pin the TMP36's Vout (sense) pin is connected to
// the resolution is 10 mV / degree centigrade with a
// 500 mV offset to allow for negative temperatures

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#define VOLTAGE 3.3F
#define tmpPin (A14)

class TMP36
{

private:
public:
  int analogPin;
  float voltage, volts, celsius, fahrenheit, kelvin;
  TMP36(int, float);
  void update();
  float getVolts();
  float getCelsius();
  float getKelvin();
  float getFahrenheit();
};
#endif