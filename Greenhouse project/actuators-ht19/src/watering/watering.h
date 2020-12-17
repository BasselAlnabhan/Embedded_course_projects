/**
 * @file watering.h
 * @author Valentine and Lucas 
 * @brief This is an app implementation to control the watering system of the Greenhouse project
 * @version 0.1
 * @date 2020-11-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef WATERING_H_
#define WATERING_H_

#define WATERING_INTERVAL (100U)

/**
 * @brief This function is used to initialize the watering system app
 */
void watering_init(void);

/**
 * @brief This function is used to run the watering system logic
 */
void watering_run(void);

#endif /* WATERING_H_ */