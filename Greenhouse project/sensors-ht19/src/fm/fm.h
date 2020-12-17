/**
 * @file fm.h
 * @author Niklas Svensson (Niklas.svensson@yaelev.se)
 * @brief Application for the flow meter sensor
 * @version 0.1
 * @date 2020-11-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FM_H
#define FM_H

/**
 * @brief Function to initilize the application
 * @remark Has to be run first
 * 
 */
void fm_app_init(void);

/**
 * @brief Function for updating the values for fluid flow, and status on the canbus in mililiter
 * @remark Values set on canbus are since last function call, call function every secondon to get
 * mililiter/second
 * @remark Until valid calibration values are recived status will be set to UNITILIZED and flow set to 0
 * @remark If read values are outside the calibration range the flow will be set to 0
 */
void fm_app_update(void);

#endif /* FM_H */