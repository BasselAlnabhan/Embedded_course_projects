### Ambient temperature and humidity sensor requirements

* [ReqId:001] It shall be possible to initilize the module
* [ReqId:002] <sensor_status> should be UNINITILIZED until it gets valid calibration values
* [ReqId:003] The module shall wait until gets valid value for <temperature_calibration_min> and <temperature_calibration_max>
* [ReqId:004] The module shall wait until gets valid value for <humidity_calibration_min> and <humidity_calibration_min>
* [ReqId:005] It shall be possible to get <temperature_calibration_min> from CAN bus
* [ReqId:006] It shall be possible to get <temperature_calibration_max> from CAN bus
* [ReqId:007] It shall be possible to get <humidity_calibration_min> from CAN bus
* [ReqId:008] It shall be possible to get <humidity_calibration_max> from CAN bus
* [ReqId:009] It Shall be possible to read current temperature <temperature_value> as a float number every 1000ms
* [ReqId:010] It shall be possible to read current humidity <humidity_value> as a integer number every 1000ms
* [ReqId:011] It shall be possible to convert <humidity_value> to a percentage representing the humidity level <humidity_level>
* [ReqId:012] If <temperature_value> is not in range of <temperature_calibration_min> and <temperature_calibration_max> the status of the sensor
            <sensor_status> shall be set to ERROR
* [ReqId:013] If <humidity_value> is not in range of <humidity_calibration_min> and <humidity_calibration_max> the status of the sensor
            <sensor_status> shall be set to ERROR
* [ReqId:014] It shall be possible to send <temperature_value> to the CAN bus every 1000ms
* [ReqId:015] It shall be possible to send <humidity_level> to the CAN bus every 1000ms
* [ReqId:016] It shall be possible to send <sensor_status> to the CAN bus every 1000ms

