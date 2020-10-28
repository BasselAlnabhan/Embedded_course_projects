# SPI - Control The LED on Teensy By ESP32

In this program, the built-in LED on Teensy can be controlled by ESP32 via SPI.

## Connections

Teensy(Slave) ........... ESP32(Master)
Vin  ... ... ... ... ...  USB

SS(10) .................. SS(33)

MISO(12) ................ MOSI(18)

MOSI(11) ................ MISO(19)

SCK(14) ................. SCK(5)

GND ..................... GND

