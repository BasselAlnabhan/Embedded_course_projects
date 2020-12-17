#ifndef WATERING_H
#define WATERING_H

#define WATERING_INTERVAL (100U)

/**
 * @brief This function is used to initialize the watering application
 * 
 */
void watering_init(void);

/**
 * @brief This function is used to update the values and statuses of the flow meter, soil moisture and water level sensors.
 * 
 */
void watering_run(void);

#endif /* WATERING_H */