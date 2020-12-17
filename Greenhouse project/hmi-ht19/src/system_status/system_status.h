/**
 * @file system_status.h
 * @author Adrian Morkvist
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H

#include <pitches.h>

#define SYSTEM_STATUS_INTERVAL (10U)

/**
 * @brief initialize function for system status application
 * 
 */
void system_status_init(void);

/**
 * @brief function for running the system status application in scheduler
 * 
 */
void system_status_run(void);

#endif /* SYSTEM_STATUS_H */
