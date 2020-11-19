#ifndef CLIENT_H
#define CLIENT_H

#include <canbus.h>
#include <Arduino.h>

#define PASSWORD_LENGTH (8U)

request_t get_user_request(void);
void recieve_new_session(response_t *res);
float get_the_temp(response_t *res);

#endif /* CLIENT_H */