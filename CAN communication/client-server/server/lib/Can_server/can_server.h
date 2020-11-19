#ifndef CAN_SERVER_H
#define CAN_SERVER_H

#include <canbus.h>

response_t authenticate(request_t *req);
response_t get_temp(request_t *req);
#endif /* CAN_SERVER_H */