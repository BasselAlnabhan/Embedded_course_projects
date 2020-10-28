#include <Metro.h>
#include <Arduino.h>

uint8_t ledState = LOW;
Metro metro = Metro(1000U);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (metro.check())
  {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
  }
}