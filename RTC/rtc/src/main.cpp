#include <datetime.h>
#include <Arduino.h>
#include <TimeLib.h>

void setup()
{
    Serial.begin(9600);
    while (!Serial.dtr())
    {
        delay(100);
    }
}

void loop()
{
    Serial.println("S) Set datetime");
    Serial.println("G) Get datetime");
    Serial.print("Enter the command: ");
    char menu = readCommand();

    if (menu == 'S')
    {
        char buffer[DATETIME_LENGTH] = {};

        Serial.printf("Enter the current datetime (YYYY-MM-DD HH:MM:SS): ");
        datetime_t datetime = strtotime(readString(buffer));
        setTime(datetime.hour, datetime.minute, datetime.second, datetime.day, datetime.month, datetime.year);
        Serial.print("\n\n");
    }
    else if (menu == 'G')
    {
        Serial.print("Current datetime: ");
        Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n\n", year(), month(), day(), hour(), minute(), second());
    }
}