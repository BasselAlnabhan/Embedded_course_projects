#include <Arduino.h>

void setup() {
  pinMode(PIND0, OUTPUT);
}

void loop() {
  digitalWrite(PIND0, LOW);
  delay(10);
  digitalWrite(PIND0, HIGH);
  delay(10);
}