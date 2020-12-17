# The main requirments extracted from the customer requirments
----------------------------------------------------------------
* **[REQID:001]** The heater and the ventilation system shoud react on the temperature sensor's reading.
* **[REQID:002]** The ventilation system should react on the humidity sensor's reading.
* **[REQID:003]** The lamp should react on the intensity sensor's reading.
* **[REQID:004]** The waterpump and the ventilation system should react on the soil moisture sensor's reading.
* **[REQID:005]** The watervalve should react on the liquid sensor's rading of the water level in the small water tank.

# The specific requirements for the different devices
-------------------------------------------------------

## Air fan
___________
* **[ReqID:001]** It shall be possible to initialize the <fan> as soon as it gets a valid values, otherwise the <fan_status> shall be
            UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <temperature_target_min_val>, <temperature_target_max_val>,
             The <humidity_target_min_val>, <humidity_target_max_val>, <soilmoisture_target_min_val>, and the <soilmoisture_target_max_val> from CAN bus every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <temperature>, <humidity>, <soilmoisture_value>, <soilmoisture_sensor_status> and <dht_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible to turn the <fan> ON/OFF
* **[ReqID:005]** If <temperature>/<humidity>/<soilmoisture_value> is more than <temperature_target_max_val>/<humidity_target_max_val>/<soilmoisture_target_max_val> respectively the <fan_state> shall be ON, otherwise the <fan_state> shall be OFF
* **[ReqID:006]** It shall be possible to send <fan_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to set <fan_status> to OK if everything is ok, otherwise the <fan_status> shall be ERROR.
* **[ReqID:008]** It shall be possible to overwrite <fan_status>.

## Heater                                                                                                     
___________

 _When We need to prioritate, the temperature is the most important factor, then comes the soilmoisture, and last is the humidity_

* **[ReqID:001]** It shall be possible to initialize the <heater> as soon as it gets a valid values, otherwise the <heater_status> shall be UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <temperature_target_min_val> and <temperature_target_max_val>,
            and The <humidity_target_min_val> and <humidity_target_max_val> from CAN bus every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <temperature>, <humidity> and <dht_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible to turn the <heater> ON/OFF
* **[ReqID:005]** If <temperature>/<humidity> is less than <temperature_target_min_val>/<humidity_target_min_val> respectively
                the <heater_state> shall be turned ON and the <heater_status> should be OK
            else if <temperature>/<humidity> is more than <temperature_target_max_val>/<humidity_target_max_val> respectively
                the <heater_state> shall be turned OFF and the <heater_status> shall be OK
            else the <heater_state> shall be turned OFF and the <heater_status> shall be NOT_WORKING
* **[ReqID:006]** It shall be possible to send <heater_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to set <heater_status> to OK if everything is ok, otherwise the <heater_status> shall be ERROR.
* **[ReqID:008]** It shall be possible to overwrite <heater_status>.

## Lamp
________

* **[ReqID:001]** It shall be possible to initialize the <lamp> as soon as it gets a valid values, otherwise the <lamp_status> shall be
            UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <intensity_target_min_val>, <intensity_target_max_val> every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <intensity> and <intensity_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible to turn the <lamp> ON/OFF
* **[ReqID:005]** If <intensity> is less than <intensity_target_min_val>, the <lamp_state> shall be ON, otherwise the <lamp_state> shall be OFF
* **[ReqID:006]** It shall be possible to send <lamp_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to overwrite <lamp_status>.

## WaterPump
_____________

* **[ReqID:001]** It shall be possible to initialize the <waterpump> as soon as it gets a valid values, otherwise the <waterpump_status> shall be UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <soilmoisture_target_min_val> and <soilmoisture_target_max_val> from CAN bus every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <soilmoisture_value> and <soilmoisture_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible to turn the <waterpump> ON/OFF
* **[ReqID:005]** If <soilmoisture_value> is less than <soilmoisture_target_max_val> the <waterpump> shall be ON, otherwise the <waterpump> shall be OFF
* **[ReqID:006]** It shall be possible to send <waterpump_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to set <waterpump_status> to OK if everything is ok, otherwise the <waterpump_status> shall be ERROR.
* **[ReqID:008]** It shall be possible to overwrite <waterpump_status>.

## Watervalve
______________

* **[ReqID:001]** It shall be possible to initialize the <watervalve> as soon as it gets a valid values, otherwise the <watervalve_status> shall be UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <liquid_level_target_min_val> and <liquid_level_target_max_val> from CAN bus every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <liquid_level_value> and <liquid_level_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible open and close the <watervalve>.
* **[ReqID:005]** If <liquid_level_value> is less than <liquid_level_target_min_val> the <watervalve_state> shall be OPEN, otherwise the <watervalve> shall be CLOSE
* **[ReqID:006]** It shall be possible to send <watervalve_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to set <watervalve_status> to OK if everything is ok, otherwise the <watervalve_status> shall be ERROR.
* **[ReqID:008]** It shall be possible to overwrite <watervalve_status>.

## Window ventilator
_____________________

* **[ReqID:001]** It shall be possible to initialize the <window> as soon as it gets a valid values, otherwise the <window_statis> shall be UNINITIALIZED.
* **[ReqID:002]** It shall be possible to read The <temperature_target_min_val>, <temperature_target_max_val>,
             The <humidity_target_min_val>, <humidity_target_max_val>, <soilmoisture_target_min_val>, and the <soilmoisture_target_max_val> from CAN bus every <read_interval>.
<soilmoisture_target_max_val> from CAN bus every <read_interval>.
* **[ReqID:003]** It shall be possible to read the <temperature>, <humidity>, <soilmoisture_value>, <soilmoisture_sensor_status> and <dht_sensor_status> from CAN bus every <read_interval>.
* **[ReqID:004]** It shall be possible to open and close the <window>
* **[ReqID:005]** If <temperature>/<humidity>/<soilmoisture_value> is more than <temperature_target_max_val>/<humidity_target_max_val>/<soilmoisture_target_max_val> respectively
                the <window_state> shall be OPEN else the <window_state> shall be CLOSE 
* **[ReqID:006]** It shall be possible to send <window_status> signal to the CAN bus every <write_interval>.
* **[ReqID:007]** It shall be possible to set <window_status> to OK if everything is ok, otherwise the <window_status> shall be ERROR.
* **[ReqID:008]** It shall be possible to overwrite <window_status>.
