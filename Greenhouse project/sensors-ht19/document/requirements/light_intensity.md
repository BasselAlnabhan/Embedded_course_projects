# Light intensity sensor requirements

* [ReqID:001] It shall be possible to initialize the module
* [ReqId:002] <ldr_status> should be UNINITILIZED until the module gets initialized
* [ReqId:004] The module shall wait until it gets initialized
* [ReqID:005] It shall be possible to read current light intensity analog value <ldr_analog_value> as a integer number every 1000ms
* [ReqID:006] It shall be possible to convert <ldr_analog_value> to a 10 bits digital value <ldr_value>
* [ReqID:007] It shall be possible to convert <ldr_value> to a light intensity value <ldr_intensity_value> between 0 and 100
* [ReqID:008] It shall be possible to send <ldr_intensity_value> to the CAN bus every 1000ms
* [ReqID:009] It shall be possible to send <ldr_status> to the CAN bus