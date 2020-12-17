#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

void set_water_level(uint8_t value);

/**
 * @brief Set the water level.y sensor status.
 *
 * @param value The value only can be UNINITIALIZED | OKAY | ERROR.
 */
void set_water_level_sensor_status(uint8_t value);

#endif /* CANBUS_H */
