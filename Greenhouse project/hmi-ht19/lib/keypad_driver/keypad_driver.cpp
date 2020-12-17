/**
 * @file keypad_driver.cpp
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-11-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <Keypad.h>
#include <keypad_driver.h>

const int ROW_NUM = 4;    //four rows
const int COLUMN_NUM = 3; //three columns
unsigned long startTime;
static byte kpadState;

//the keypad layout.
static char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

uint8_t pin_rows[ROW_NUM] = {38, 33, 34, 36};  //connect to the row pinouts of the keypad (these are set for teensy pins)
uint8_t pin_column[COLUMN_NUM] = {37, 39, 35}; //connect to the column pinouts of the keypad (these are set for teensy pins)

//initialize the keypad
static Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

static void keypadEvent_num(KeypadEvent key)
{
    kpadState = keypad.getState();
} // end keypad events

void keypad_driver_init(void)
{
    keypad.addEventListener(keypadEvent_num); // Add an event listener.
    keypad.setHoldTime(500);                  // Default is 1000mS
}

char keypad_driver_get_key(void)
{
    return keypad.getKey();
}

key_state_t keypad_driver_get_state(void)
{
    return (key_state_t)keypad.key->kstate;
}