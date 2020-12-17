# Requirements

## Menu
[ReqID:001] It shall have a main menu and sub-menus for all the components.
[ReqID:002] It shall be possible to go back/cancel in the sub-menus.
[ReqID:003] It shall have an option to overwrite the logic state of all the actuators.
[ReqID:004] It shall be possible to calibrate the whole system.
[ReqID:005] It shall be possible to initialize this module.
[ReqID:006] It shall be possible to read all the data from CAN.
[ReqID:007] It shall be possible to dynamically create a system menu based on the data recieved from CAN and a JSON file. (python based)

## Status
[ReqID:001] It shall be possible to classify the statuses of all the components in the system as three different classes, ERROR, WARNING, OK.
[ReqID:002] As long as there is no error in any of the components in the system, the status will be "OK", otherwise it will depend on the classification class.

### RGB
[ReqID:001] It shall be possible to initialize the RGB led.
[ReqID:002] It shall be possible to get the status of the RGB led.
[ReqID:004] It shall be possible to send the status signal to the CAN.
[ReqID:005] It shall be possible to set the default RGB led color to YELLOW.
[ReqID:006] It shall be possible to change the color of LED to GREEN, RED, YELLOW.
[ReqID:007] It shall be possible to read the status of all the nodes from CAN every 500ms.
[ReqID:008] If the system status is OK, the RGB led color will be GREEN.
[ReqID:009] If the system status is WARNING, the RGB led color will be YELLOW.
[ReqID:010] If the system status is ERROR, the RGB led color will be RED.

### Buzzer
[ReqID:001] It shall be possible to initialize the buzzer.
[ReqID:002] It shall be possible to get the status of the buzzer.
[ReqID:003] It shall be possible to send the status signal to the CAN.
[ReqID:004] If the system status is OK, the buzzer will be silent (pin out = LOW).
[ReqID:005] If the system status is WARNING, the buzzer will beep with a low frequency continously.
[ReqID:006] If the system status is ERROR, the buzzer will beep continously with a high frequency.
[ReqID:007] It shall beep when the calibration values have been entered into the HMI.
[ReqID:008] It shall be possible to send the status to the CAN every 1000ms.

### Keypad
[ReqID:001] It shall be possible to initialize the keypad. 
[ReqID:002] It shall be possible to get the status of the keypad.
[ReqID:003] It shall be possible to send the status signal to the CAN.
[ReqID:004] It shall be possible to enter numbers from 0-9
[ReqID:005] It shall be possible to enter two special characters (* and #)
[ReqID:006] It shall be possbile to confirm inputs via "#"
[ReqID:007] It shall be possible to erase inputs via "*"

### Display
[ReqID:001] It shall be possible to initialize the display.
[ReqID:002] It shall be possible to draw a straight line on the display.
[ReqID:003] It shall be possible to send the status signal to the CAN.
[ReqID:004] It shall be possible to draw characters on the display
[ReqID:005] It shall be possible to view the current real-world date and time.
[ReqID:006] It shall be possible to view the menu.
[ReqID:007] It shall be possible to draw a circle on the display.
[ReqID:008] It shall be possible to view the status in text with a colored circle which has the same color as the current status of the system. If there is a WARNING or ERROR then it will also display the affected component.
[ReqID:009] It shall be possible to get the status of the Display.
[ReqID:010] It shall be possible to update the content of the display every 300ms.

### EEPROM
[ReqID:001] It shall be possible to initialize the EEPROM.
[ReqID:002] It shall be possible to get the status of the EEPROM.
[ReqID:003] It shall be possible to send the status signal to the CAN.
[ReqID:004] It shall be possible to read the calibration value for each component in the system.
[ReqID:005] It shall be possible to set the calibration value for each component in the system.
[ReqID:006] It shall be possible to overwrite the calibration value for each component in the system.
