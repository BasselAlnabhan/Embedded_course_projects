#include <canbus.h>
#include <Arduino.h>
#include <can_server.h>

static response_t handle_request(request_t *req);
extern uint32_t sessionID;
extern float temp;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    canbus_begin();
}

void loop()
{
    request_t request = get_request();

    response_t response = handle_request(&request);

    send_response(&response);
}

static response_t handle_request(request_t *req)
{
    response_t res = {.length = BAD_REQUEST};

    if (req->length == strlen("authentication"))
    {
        res = authenticate(req);
    }
    else if (req->length == 36U)
    {
        res = get_temp(req);
    }
    else if (req->length == 37U)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        res = {.length = 37U};
    }
    else if (req->length == 38U)
    {
        digitalWrite(LED_BUILTIN, LOW);
        res = {.length = 38U};
    }
    else if (req->length == strlen("close"))
    {
        memset(&sessionID, 0, sizeof(sessionID));
    }
    return res;
}