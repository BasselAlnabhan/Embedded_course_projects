#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <stdint.h>

#define DATETIME_LENGTH 20

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} datetime_t;

char readCommand();
char *readString(char *buffer);
datetime_t strtotime(char *buffer);

#endif