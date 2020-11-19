#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

// Size of the buffer
#define BUFFER_SIZE (8U)

/**
 * @brief This function is used to reset the required variables like head and tail of the buffer.
 *        The head is used for reading data and the tail is used for writing data. 
 */
void circular_buffer_init(void);

/**
 * @brief This function is used to check if the buffer is full or not
 * 
 * @return true if the bufer is full
 * @return false if the buffer is not full
 */
bool circular_buffer_isfull(void);

/**
 * @brief This function is used to peek data in the buffer.
 *        Peeking means; read the data but don't change the head.
 * 
 * @return uint8_t The peeked data.
 */
uint8_t circular_buffer_peek(void);

/**
 * @brief This function is used to read data from the buffer
 * 
 * @return uint8_t The read data.
 */
uint8_t circular_buffer_read(void);

/**
 * @brief This function returns the number of available data in the buffer which can be read.
 * 
 * @return uint8_t number of available elements
 */
uint8_t circular_buffer_available(void);

/**
 * @brief This function is used to write data to the buffer.
 * 
 * @param value The data which is supposed to be written to the buffer.
 */
void circular_buffer_write(uint8_t value);

#endif /* CIRCULAR_BUFFER_H */