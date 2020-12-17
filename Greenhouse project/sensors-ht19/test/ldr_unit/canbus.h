#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Set the light intensity percentage.
 *
 * @param value The light intensity percentage should be between 0 and 100.
 */
void set_light_intensity(uint8_t value);

/**
 * @brief Set the light intensity sensor status.
 *
 * @param value The value only can be UNINITIALIZED | OKAY | ERROR.
 */
void set_light_intensity_sensor_status(uint8_t value);

#endif /* CANBUS_H */