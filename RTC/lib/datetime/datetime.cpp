#include <Arduino.h>
#include <datetime.h>

/**
 * Datetime format: YYYY-MM-DD HH:MM:SS
**/

datetime_t strtotime(char *buffer)
{
    datetime_t datetime = {.year = 0, .month = 0, .day = 0, .hour = 0, .minute = 0, .second = 0};
    sscanf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", &datetime.year, &datetime.month, &datetime.day, &datetime.hour, &datetime.minute, &datetime.second);
    return datetime;
}

char readCommand()
{
    char command = 0;

    while ((command != 'S') && (command != 'G') && (command != '\n'))
    {
        if (Serial.available())
        {
            command = toupper(Serial.read());
        }
    }

    Serial.printf("%c\n\n", command);

    return command;
}

char *readString(char *buffer)
{
    uint8_t i = 0;

    while (i < DATETIME_LENGTH)
    {
        if (Serial.available())
        {
            char temp = Serial.read();
            if (temp == '\n')
            {
                break;
            }
            else if (isdigit(temp) || isspace(temp) || (temp == '-') || (temp == ':'))
            {
                Serial.print(temp);
                buffer[i] = temp;
                i++;
            }
        }
    }

    buffer[DATETIME_LENGTH - 1] = 0;
    Serial.clear();

    return buffer;
}