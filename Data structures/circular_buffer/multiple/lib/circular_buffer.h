#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct circular_buffer_t circular_buffer_t;

circular_buffer_t *circular_buffer_create(uint8_t size_of_buffer);

bool circular_buffer_read(circular_buffer_t *circular_buffer, uint8_t *data);

bool circular_buffer_peek(circular_buffer_t *circular_buffer, uint8_t *data);

bool circular_buffer_available(circular_buffer_t *circular_buffer, uint8_t *available);

bool circular_buffer_write(circular_buffer_t *circular_buffer, uint8_t data);

bool circular_buffer_isfull(circular_buffer_t *circular_buffer, bool *full);

bool circular_buffer_reset(circular_buffer_t *circular_buffer);

void circular_buffer_destroy(circular_buffer_t *circular_buffer);

#endif /* CIRCULAR_BUFFER_H */