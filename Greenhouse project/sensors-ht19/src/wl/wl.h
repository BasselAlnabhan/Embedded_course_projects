/**
 * @file water_level.h
 * @author Alain (AlainDlcTVOP)
 * @brief The water level application.
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WL_H
#define WL_H

/**
 * @brief  function to initialized the driver application
 * 
 */
void water_level_app_init(void);
/**
 * @brief update the canbus values and set the status
 * 
 */
void water_level_app_update(void);

#endif /* WL_H */