#include <Arduino.h>

#define BLUE_PIN 2
#define GREEN_PIN 3
#define RED_PIN 4

void setup()
{
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);

	Serial.begin(9600);
	while (!Serial)
		;
}

void loop()
{
	uint8_t red = random() % 256;
	uint8_t green = random() % 256;
	uint8_t blue = random() % 256;

	Serial.printf("R = 0x%02X, G = 0x%02X, B = 0x%02X\n", red, green, blue);

	analogWrite(RED_PIN, red);
	analogWrite(GREEN_PIN, green);
	analogWrite(BLUE_PIN, blue);

	delay(1000);
}