#include <Arduino.h>
#include <IntervalTimer.h>

IntervalTimer timer;
uint8_t ledState = LOW;

void blink(void)
{
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  timer.begin(blink, 1000000); // blink is called every second
}

void loop()
{
}