/**
 * @file ldr.h
 * @author Stefan Edenholm (stefan.edenholm@yaelev.se)
 * @brief App for the light intensity sensor
 * @version 0.1
 * @date 2020-11-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef LDR_H
#define LDR_H

/**
 * @brief Initializing the light intensity app.
 * 
 * @remark Has to be run first.
 */
void ldr_app_init(void);

/**
 * @brief Update the light intensity app.
 */
void ldr_app_update(void);

#endif /* LDR_H */