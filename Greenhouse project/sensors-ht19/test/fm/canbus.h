#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

void set_flow_rate(uint16_t value);
void set_flow_meter_sensor_status(uint8_t value);

#endif /* CANBUS_H */