/**
 * @file fm_driver.h
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief Driver for flow meter YF-S201
 * @version 0.1
 * @date 2020-10-26
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FM_DRIVER_H
#define FM_DRIVER_H

#include <stdint.h>

/**
 * @brief Function to initialized the sensor.
 * @remark Function has to be run first
 */
void flow_meter_driver_init(void);

/**
 * @brief Function for calculating the total volume of fluid since last function call
 * 
 * @param flow Address to the output for volume reading in mililiters.
 * @remark Call this function again to determin flow measuring time
 * i.e call function every 1000ms to get mililiter/second
 * @remark OVERFLOW_ERROR will occur after 21845 pulses or 48,5 liters of fluid since last function call
 * when error occur the pulse counter will be reset
 * @return OKAY if successful calculation else an ERROR
 */
uint8_t flow_meter_driver_read(uint16_t *flow);

#endif
