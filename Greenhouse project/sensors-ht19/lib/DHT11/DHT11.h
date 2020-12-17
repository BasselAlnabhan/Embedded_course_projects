#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>
#include <stdbool.h>

#define DHT11_PIN (35)
#define DHT11_OKAY (0U)
#define DHT11_WAIT (1U)
#define DHT11_ERROR (2U)

/**
 * @brief This function is used to initialize the driver
 * 
 */
void dht11_init(void);

/**
 * @brief This function is used to read temperature and humidity from the sensor. This function should be called every 20 milliseconds.
 * 
 * @param temperature Address of a float variable to fill it with the read temperature. 
 * @param humidity Address of an uint8_t variable to fill it with the read humidity in percentage.
 * @return uint8_t The status of the sensor. It can be DHT11_OKAY, DHT11_WAIT or DHT11_ERROR.
 */
uint8_t dht11_read(float *temperature, uint8_t *humidity);

#endif /* DHT11_H */