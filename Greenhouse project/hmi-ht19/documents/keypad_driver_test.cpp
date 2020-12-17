#include <bsp.h>
#include <keypad_driver.h>

#define MAX_ENTRIES 10
#define DOT_MENU 1

char calibration_values[MAX_ENTRIES] = {};
int counter = 0;

void setup()
{
    Serial.begin(9600);
    keypad_driver_init();
}

void loop()
{
    char key = keypad_driver_get_key();
    //if were in specific menu where we want a dot instead of a star
    // we set DOT_MENU to 1 and do this (if) statement
    //in order to change the star to a dot.
    //in the case we want the star or dot to act as an eraser, we hold that button.

    if (key)
    {
        if (DOT_MENU && key == '*' && counter < MAX_ENTRIES)
        {
            key = '.';
            calibration_values[counter] = key;
            Serial.printf("\n%s\n", calibration_values);
            counter++;
        }
        else if (counter < MAX_ENTRIES)
        {
            calibration_values[counter] = key;
            Serial.printf("\n%s\n", calibration_values);
            counter++;
        }
        else if (counter == MAX_ENTRIES)
        {
            Serial.printf("You have reached the maximum entries, erase or enter to continue.");
        }
    }
    if (keypad_driver_get_state() == KEY_HOLD && counter > 0)
    {
        //we wanna have so that this is inside where we have an array, it erases the last input. (if we keep holding it will erase until the array is 0)
        calibration_values[counter] = '\0';
        Serial.printf("\n%s\n", calibration_values);
        counter--;
        delay(500);
    }
}