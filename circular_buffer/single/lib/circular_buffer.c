#include <circular_buffer.h>

#if ((BUFFER_SIZE < 8U) || (BUFFER_SIZE > 255U))
#error BUFFER_SIZE should be an integer between 8 and 255
#endif

static uint8_t tail = 0U;
static uint8_t head = 0U;
static bool full = false;
static uint8_t buffer[BUFFER_SIZE] = {};

void circular_buffer_init(void)
{
    tail = 0U;
    head = 0U;
    full = false;
}

void circular_buffer_write(uint8_t value)
{
    buffer[tail] = value;
    tail = (tail + 1U) % BUFFER_SIZE;

    if (full) // Overwrite
    {
        head = (head + 1U) % BUFFER_SIZE;
    }

    full = (head == tail);
}

uint8_t circular_buffer_read(void)
{
    uint8_t data = buffer[head];

    if (full || (head != tail)) // Not empty
    {
        head = (head + 1) % BUFFER_SIZE;
    }

    full = false;

    return data;
}

bool circular_buffer_isfull(void)
{
    return full;
}

uint8_t circular_buffer_peek(void)
{
    return buffer[head];
}

uint8_t circular_buffer_available(void)
{
    uint8_t available = BUFFER_SIZE;

    if (!full)
    {
        if (tail >= head)
        {
            available = tail - head;
        }
        else
        {
            available -= (head - tail);
        }
    }

    return available;
}