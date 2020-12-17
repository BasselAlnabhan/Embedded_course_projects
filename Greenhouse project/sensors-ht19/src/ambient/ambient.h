#ifndef AMBIENT_H
#define AMBIENT_H

#define AMBIENT_INTERVAL (20U)

/**
 * @brief This function is used to initialize the ambient application
 * 
 */
void ambient_init(void);

/**
 * @brief This function is used to read the DHT11 sensor and send data to the CAN bus.
 *        This function shall be run every 20 milliseconds.
 * 
 */
void ambient_run(void);

#endif /* AMBIENT_H */