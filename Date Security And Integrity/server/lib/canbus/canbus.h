#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>

#define MESSAGES (13U) // Number of the messages
#define DATA_LENGTH (7U * MESSAGES)

enum Request
{
    AUTH,
    CLOSE,
    TEMPERATURE,
    TURN_LED_ON,
    TURN_LED_OFF
};

enum Status
{
    OKAY = 1,
    UNAUTH,
    EXPIRED,
    TIMEOUT,
    HASH_ERROR,
    BAD_REQUEST,
    UNKNOWN_ERROR
};

typedef struct
{
    uint8_t length;
    uint8_t data[DATA_LENGTH];
} request_t;

typedef struct
{
    uint8_t length;
    uint8_t data[DATA_LENGTH];
} response_t;

void canbus_begin(void);
request_t get_request(void);
void send_response(response_t *res);

#endif /* CANBUS_H */