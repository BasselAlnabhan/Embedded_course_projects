/**
 * @file sm_app.h
 * @author Mohammed Fazlur Rahman (raseleee02@gmail.com)
 * @brief Application for the soil moisture sensor
 * @version 0.1
 * @date 2020-11-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SM_H
#define SM_H

/**
 * @brief Function to initilize the application
 * @remark Has to be run first
 * 
 */
void sm_app_init(void);

/**
 * @brief Function for updating the values 
 * @remark If read values are outside the calibration range the moisture will be set to 0
 */
void sm_app_update(void);

#endif /* SM_H */