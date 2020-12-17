### water level sensor requirement

* [ReqId:001] It shall be possible to initialize the module
* [ReqId:002] <water_level_status> should be UNINITILIZED until the module gets initialized
* [ReqId:003] The module shall wait until it gets initialized
* [ReqId:004] It shall be possible to read the analog value of the water level sensor <analog_level_value> every 1000ms
* [ReqId:005] It shall be possible to convert <analog_level_value> to a percentage representing the water level <water_level>
* [ReqId:006] It shall be possible to send <water_level> to the CAN bus every 1000ms
* [ReqId:007] It shall be possible to send <water_level_status> to the CAN bus every 1000ms