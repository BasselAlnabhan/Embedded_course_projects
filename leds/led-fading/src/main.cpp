#include <Arduino.h>

int8_t fadeAmount = 5;
int16_t brightness = 0;

void setup()
{
  pinMode(PIN_A9, OUTPUT);
}

void loop()
{
  analogWrite(PIN_A9, brightness);

  brightness += fadeAmount;

  if (brightness <= 0 || brightness >= 255)
  {
    fadeAmount = -fadeAmount;
  }

  delay(30);
}