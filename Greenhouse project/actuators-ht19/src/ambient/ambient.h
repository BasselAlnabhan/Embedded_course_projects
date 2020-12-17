/**
 * @file ambient.h
 * @author Bassel Alnabhan (basselalnabhan@gmail.com)
 * @brief This is an app interface to conrol the ambient system in the Greenhouse project.
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef AMBIENT_H
#define AMBIENT_H

#define AMBIENT_INTERVAL (100U)

/**
 * @brief This function is used to initialize the ambient app
 */
void ambient_init(void);

/**
 * @brief This function is used to run the ambient app logic
 */
void ambient_run(void);

#endif /* AMBIENT_H */