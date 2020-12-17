# List Of Signals (HMI)

| Signal name | Type | Length | Values | Calibration? | Overwrite? | Set Nodes | Get Nodes |
|---|---|---|---|---|---|---|---|
|HMI NODE| | | | | | | |
| display_status | unsigned int | 4 bits | [0 to 7] | No | No |HMI | communication |
| keypad_status | unsigned int | 4 bits | [0 to 7] | No |  No |HMI | communication |
| buzzer_status | unsigned int | 4 bits | [0 to 7] | No |  No |HMI | communication |
| rgb_status | unsigned int | 4 bits | [0 to 7] | No |  No |HMI | communication |
| eeprom_status | unsigned int | 4 bits | [0 to 7] | No |  No |HMI , Terminal | communication |
| general_status | unsigned int | 4 bits | [0 to 7] | No | yes; by HMI | actuators ,HMI | 
|COMMUNICATION NODE|---|---|---|---|---|---|---|
| sd_card_status | unsigned int | 4 bits | [0 to 7] | No | No | communication |HMI , Terminal |
| sd_card_available | unsigned int | 4 bits | [0 kb, 2048 kb] | No | No | communication |HMI |
|ACTUATORS NODE| | | | | | | |
| window_ventilator_status | unsigned int | 4 bits | [0 to 7] | No |  NO | actuators | communication ,HMI , Terminal |
| window_ventilator_state | unsigned int | 4 bits | [01, 00, 10, 11] | No | yes; by HMI , Terminal | actuators ,HMI , Terminal | communication ,HMI , Terminal |
| water_pump_status | unsigned int | 4 bits | [0 to 7] | No |  NO | actuators | communication ,HMI , Terminal |
| water_pump_state | unsigned int | 4 bits | [01, 00, 10, 11] | No |  yes; by HMI , Terminal | actuators ,HMI , Terminal | communication ,HMI , Terminal |
| heating_status | unsigned int | 7 bit | [0 to 7] | no |  NO | actuators | communication ,HMI , Terminal |
| heating_state | unsigned int | 4 bits | [01, 00, 10, 11] | No |  yes; by HMI , Terminal | actuators ,HMI , Terminal | communication ,HMI , Terminal |
| air_fan_status | unsigned int | 7 bits | [0 to 7] | No | NO | actuators | communication ,HMI , Terminal |
| air_fan_state | unsigned int | 2 bits | [01, 00, 10, 11] | No |  yes; by HMI , Terminal | actuators ,HMI , Terminal | communication ,HMI , Terminal |
| water_valve_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | actuators | communication ,HMI , Terminal |
| water_valve_state | unsigned int | 2 bits | [01, 00, 10, 11] | No |  yes; by HMI , Terminal | actuators ,HMI , Terminal | communication ,HMI , Terminal |
|SENSORS NODE| | | | | | | |
| fluid_sen_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | sensors | sensors , actuators , communication ,HMI , Terminal |
| fluid_sen_cal_value_max | unsigned int | 7 bits | [1 to 100] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| fluid_sen_cal_value_min | unsigned int | 7 bits | [0 to 99] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| temp_sen_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | sensors ,HMI , Terminal | sensors , actuators , communication ,HMI , Terminal |
| temp_sen_cal_value_max | unsigned int | 7 bits | [6 to 35] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| temp_sen_cal_value_min | unsigned int | 7 bits | [5 to 34] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| humidity_sen_status | unsigned int | 7 bits | [0 to 7] | NO | NO | sensors ,HMI , Terminal | sensors , actuators , communication ,HMI , Terminal |
| humidity_sen_cal_value_max | unsigned int | 7 bits | [1 to 100] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| humidity_sen_cal_value_min | unsigned int | 7 bits | [0 to 99] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| light_sen_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | sensors ,HMI , Terminal | sensors , actuators , communication ,HMI , Terminal |
| light_sen_cal_value_max | unsigned int | 7 bits | [1 to 100] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| light_sen_cal_value_min | unsigned int | 7 bits | [0 to 99] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| light_sen_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | sensors ,HMI , Terminal | sensors , actuators , communication ,HMI , Terminal |
| soil_moisture_sen_cal_value_max | unsigned int | 7 bits | [1 to 100] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| soil_moisture_sen_cal_value_min | unsigned int | 7 bits | [0 to 99] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| flow_meter_sen_status | unsigned int | 7 bits | [0 to 7] | NO |  NO | sensors ,HMI , Terminal | sensors , actuators , communication ,HMI , Terminal |
| flow_meter_sen_cal_value_max | unsigned int | 7 bits | [1 to 100] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |
| flow_meter_sen_cal_value_min | unsigned int | 7 bits | [0 to 99] | Yes |  No |HMI , Terminal | sensors , communication ,HMI , Terminal |