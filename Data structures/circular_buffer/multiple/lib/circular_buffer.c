#include <circular_buffer.h>

struct circular_buffer_t
{
    uint8_t *buffer;
    uint8_t tail;
    uint8_t head;
    uint8_t size;
    bool full;
};

circular_buffer_t *circular_buffer_create(uint8_t size_of_buffer)
{
    circular_buffer_t *cbptr = NULL;
    if (size_of_buffer)
    {
        cbptr = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
        if (cbptr != NULL)
        {
            cbptr->buffer = (uint8_t *)malloc(size_of_buffer * sizeof(uint8_t));
            if (cbptr->buffer != NULL)
            {
                cbptr->size = size_of_buffer;
                cbptr->full = false;
                cbptr->head = 0U;
                cbptr->tail = 0U;
            }
            else
            {
                free(cbptr);
                cbptr = NULL;
            }
        }
    }

    return cbptr;
}

bool circular_buffer_read(circular_buffer_t *circular_buffer, uint8_t *data)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer && data)
    {
        if (circular_buffer->full || (circular_buffer->head != circular_buffer->tail))
        {
            *data = circular_buffer->buffer[circular_buffer->head];
            circular_buffer->head = (circular_buffer->head + 1U) % circular_buffer->size;
            circular_buffer->full = false;
            status = true;
        }
    }
    return status;
}

bool circular_buffer_peek(circular_buffer_t *circular_buffer, uint8_t *data)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer && data)
    {
        *data = circular_buffer->buffer[circular_buffer->head];
        status = true;
    }
    return status;
}

bool circular_buffer_write(circular_buffer_t *circular_buffer, uint8_t data)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer)
    {
        circular_buffer->buffer[circular_buffer->tail] = data;
        circular_buffer->tail = (circular_buffer->tail + 1U) % circular_buffer->size;

        if (circular_buffer->full)
        {
            circular_buffer->head = (circular_buffer->head + 1U) % circular_buffer->size;
        }

        circular_buffer->full = (circular_buffer->head == circular_buffer->tail);

        status = true;
    }
    return status;
}

bool circular_buffer_reset(circular_buffer_t *circular_buffer)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer)
    {
        circular_buffer->full = false;
        circular_buffer->head = 0U;
        circular_buffer->tail = 0U;
        status = true;
    }
    return status;
}

bool circular_buffer_isfull(circular_buffer_t *circular_buffer, bool *full)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer && full)
    {
        *full = circular_buffer->full;
        status = true;
    }
    return status;
}

bool circular_buffer_available(circular_buffer_t *circular_buffer, uint8_t *available)
{
    bool status = false;
    if (circular_buffer && circular_buffer->buffer && available)
    {
        *available = circular_buffer->size;
        if (!circular_buffer->full)
        {
            if (circular_buffer->tail >= circular_buffer->head)
            {
                *available = (circular_buffer->tail - circular_buffer->head);
            }
            else
            {
                *available -= (circular_buffer->head - circular_buffer->tail);
            }
        }
        status = true;
    }
    return status;
}

void circular_buffer_destroy(circular_buffer_t *circular_buffer)
{
    if (circular_buffer != NULL)
    {
        if (circular_buffer->buffer != NULL)
        {
            free(circular_buffer->buffer);
        }
        free(circular_buffer);
    }
}