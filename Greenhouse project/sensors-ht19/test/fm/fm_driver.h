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

void flow_meter_driver_init(void);

uint8_t flow_meter_driver_read(uint16_t *flow);

#endif
