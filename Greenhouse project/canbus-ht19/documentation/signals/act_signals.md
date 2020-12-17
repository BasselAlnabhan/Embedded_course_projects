### CAN_Signals_actuators19

| Driver | Signal name  | Signal type  | Length of signal  | value range  | short description  | Calibration val?  | overwrite signal?  | setters  | getters  | how often  |
|---|---|---|---|---|---|---|---|---|---|---|
| Lamp | light_intensity | uint8_t | 7 bit | 0-100 | the intensity of light | No | No | sens | act | read_interval
| Lamp |  lamp_status   |    uint8_t     |       7 bit        |     ON-OFF    | status of lamp      |        No          |         Yes         |     act   |    hmi    |      write_interval |
| Lamp |intensity_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of light sensor |        No          |         No          |     act   |    hmi    |      read_interval |
| Waterpump | soilmoisture_value   |    uint8_t     |       7 bit        |     0-100     | value of soilmoisture |        No          |         No          |     sens  |    act    |      read_interval      |
| Waterpump |soilmoisture_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | the status of soilmoisture sensor |        No          |         No          |     sens   |    act    |      read_interval     | 
| Waterpump |waterpump_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of waterpump |        No          |         Yes         |     act   |    hmi    |      write_interval     |
| Watervalve | liquid_level_value   |    uint8_t     |       7 bit        |     0-100     | the level of the water |        No          |          No         |    sens   |    act    |      read_interval      |
| Watervalve | liquid_level_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | the status of the water_level sensor |        No          |          No         |    sens   |    act    |      read_interval      |
| Watervalve |watervalve_status  |    uint8_t     |       7 bit        |     OK-ERR    | status of watervalve  |        No          |          Yes        |    sens   |    act    |      write_interval     |
|Window Ventilator| soilmoisture_value   |    uint8_t     |       7 bit        |     0-100    | value of soilmoisture |        No          |         No          |     sens  |    act    |      read_interval     |
|Window Ventilator| soilmoisture_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | the status of soilmoisture sensor|        No          |         No          |     sens   |    act    |      read_interval     |
|Window Ventilator|dht_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of dht sensor  |        No          |         No          |     sens  |    act    |      read_interval      |
|Window Ventilator|window_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of the window |        No          |         Yes         |     sens  |    act    |      write_interval     |
|Air fan| soilmoisture_value   |    uint8_t     |       7 bit        |     0-100    | value of soilmoisture |        No          |         No          |     sens  |    act    |      read_interval      |
|Air fan| soilmoisture_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | the status of soilmoisture sensor|        No          |         No          |     sens  |    act    |      read_interval      | 
|Air fan| dht_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of dht sensor|        No          |         No          |     sens  |    act    |      read_interval      | 
|Air fan| fan_status      |    uint8_t     |       7 bit        |     OK-ERR    | status of the fan   |        No          |         Yes         |     act   |    hmi    |      write_interval     |
|Heater| dht_sensor_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of dht sensor |        No          |         No          |     sens  |    act    |      read_interval      |
|Heater| heater_status   |    uint8_t     |       7 bit        |     OK-ERR    | status of the heater |        No          |         Yes         |     act   |    hmi    |      write_interval     |
|General Macros| temperature_target_min_val   |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      | 
|General Macros| temperature_target_max_val   |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      | 
|General Macros| humidity_target_min_val      |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      | 
|General Macros| humidity_target_max_val      |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      |  
|General Macros| soilmoisture_target_min_val  |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      | 
|General Macros| soilmoisture_target_max_val  |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      |
|General Macros| intensity_target_min_val     |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      |
|General Macros| intensity_target_max_val     |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      |
|General Macros| liquid_level_target_min_val  |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      |
|General Macros| liquid_level_target_max_val  |    uint8_t     |       7 bit        |     CONST     |                     |        Yes         |         No          |     hmi   |    act    |      read_interval      | 