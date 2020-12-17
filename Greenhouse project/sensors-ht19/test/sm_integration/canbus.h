#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

void set_soil_moisture(uint8_t value);
void set_soil_moisture_sensor_status(uint8_t value);

#endif /* CANBUS_H */