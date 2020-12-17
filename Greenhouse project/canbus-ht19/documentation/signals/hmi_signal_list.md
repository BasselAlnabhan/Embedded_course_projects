# List Of Signals

### HMI

## Display
Signal Name: display_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No ||Overwrite: No ||node set: HMI node || node get: communication node.

## Keypad
Signal Name: keypad_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: No ||node set: HMI node || node get: communication node.

## Buzzer
Signal Name: buzzer_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: No ||node set: HMI node || node get: communication node.

## RGB
Signal Name: rgb_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: No ||node set: HMI node || node get: communication node.

## EEPROM
Signal Name: eeprom_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: No ||node set: HMI node, Terminal node || node get: communication node.

## Menu
Signal Name: general_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No ||Overwrite: yes; by HMI node, node set:actuators node, HMI node || node get: communication node, HMI node || Description: It Will send the classified status of the whole system (OKAY / WARNING / ERROR).  

#################################################################################################################

### Communication

## SD Card
Signal Name: sd_card_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No ||Overwrite: No ||node set: communication node || node get: HMI node, Terminal node.

Signal Name: sd_card_available || Type: unsigned int || Lenght: 4 bits || Values: [0 kb, 2048 kb] || Calibration: No ||Overwrite: No ||node set: communication node || node get: HMI node.

#################################################################################################################

### Actuators

## Window Ventilator
Signal Name: window_ventilator_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: NO || node set: actuators node || node get: communication node, HMI node, Terminal node.

Signal Name: window_ventilator_state || Type: unsigned int || Lenght: 4 bits || Values: [01, 00, 10, 11] || Calibration: No ||Overwrite: yes; by HMI node, Terminal node || node set: actuators node, HMI node, Terminal node || node get: communication node, HMI node, Terminal node.

## Water Pump
Signal Name: water_pump_status || Type: unsigned int || Lenght: 4 bits || Values: [0 to 7] || Calibration: No || Overwrite: NO || node set: actuators node || node get: communication node, HMI node, Terminal node.

Signal Name: water_pump_state || Type: unsigned int || Lenght: 4 bits || Values: [01, 00, 10, 11] || Calibration: No || Overwrite: yes; by HMI node, Terminal node || node set: actuators node, HMI node, Terminal node || node get: communication node, HMI node, Terminal node.

## Heating
Signal Name: heating_status || Type: unsigned int || Lenght: 7 bit || Values: [0 to 7] || Calibration: no || Overwrite: NO || node set: actuators node || node get: communication node, HMI node, Terminal node.

Signal Name: heating_state || Type: unsigned int || Lenght: 4 bits || Values: [01, 00, 10, 11] || Calibration: No || Overwrite: yes; by HMI node, Terminal node || node set: actuators node, HMI node, Terminal node || node get: communication node, HMI node, Terminal node.

## Air Fan
Signal Name: air_fan_status || Type: unsigned int || Lenght: 7 bits || Values: [0 to 7] || Calibration: No ||Overwrite: NO || node set: actuators node || node get: communication node, HMI node, Terminal node.

Signal Name: air_fan_state || Type: unsigned int || Lenght: 2 bits || Values: [01, 00, 10, 11] || Calibration: No || Overwrite: yes; by HMI node, Terminal node || node set: actuators node, HMI node, Terminal node || node get: communication node, HMI node, Terminal node.

## Water Valve
Signal Name: water_valve_status || Type: unsigned int || Lenght: 7 bits || Values: [0 to 7] || Calibration: NO || Overwrite: NO || node set: actuators node || node get: communication node, HMI node, Terminal node.

Signal Name: water_valve_state || Type: unsigned int || Lenght: 2 bits || Values: [01, 00, 10, 11] || Calibration: No || Overwrite: yes; by HMI node, Terminal node || node set: actuators node, HMI node, Terminal node || node get: communication node, HMI node, Terminal node.

#################################################################################################################

### Sensors

## Fluid Level Sensor
Signal Name: fluid_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO || Overwrite: NO || node set: sensors node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: fluid_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [1 to 100] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: fluid_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 99] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

## Temp Sensor
Signal Name: temp_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO || Overwrite: NO || node set: sensors node, HMI node, Terminal node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: temp_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [6 to 35] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: temp_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [5 to 34] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

## Humidity Sensor
Signal Name: humidity_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO ||Overwrite: NO || node set: sensors node, HMI node, Terminal node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: humidity_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [1 to 100] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: humidity_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 99] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

## Light Intensity Sensor
Signal Name: light_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO || Overwrite: NO || node set: sensors node, HMI node, Terminal node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: light_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [1 to 100] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: light_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 99] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

## Soil Moisture Sensor
Signal Name: light_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO || Overwrite: NO || node set: sensors node, HMI node, Terminal node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: soil_moisture_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [1 to 100] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: soil_moisture_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 99] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

## Flow Meter Sensor
Signal Name: flow_meter_sen_status || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 7] || Calibration: NO || Overwrite: NO || node set: sensors node, HMI node, Terminal node || node get: sensors node, actuators node, communication node, HMI node, Terminal node.

Signal Name: flow_meter_sen_cal_value_max || Type: unsigned int || Lenght: 7 bits || Values|: [1 to 100] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.

Signal Name: flow_meter_sen_cal_value_min || Type: unsigned int || Lenght: 7 bits || Values|: [0 to 99] || Calibration: Yes || Overwrite: No || node set: HMI node, Terminal node || node get: sensors node, communication node, HMI node, Terminal node.