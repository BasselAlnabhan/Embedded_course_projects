#include <Arduino.h>
#include <canbus.h>
#include <client.h>

extern uint8_t aes_key[];

extern uint32_t sessionID;

void err_msg(uint8_t err_code)
{
    switch (err_code)
    {
    case UNAUTH:
        Serial.println("unauthenticated");
        break;
    case EXPIRED:
        Serial.println("Session expiered");
        break;
    case TIMEOUT:
        Serial.println("Timeout");
        break;
    case HASH_ERROR:
        Serial.println("Hash error");
        break;
    case BAD_REQUEST:
        Serial.println("Bad request");
        break;
    default:
        Serial.println("Unknown error");
        break;
    }
}

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
    request_t request = get_user_request();

    response_t response = send_request(&request);

    if ((response.length == 1U) && (response.data[0] == TIMEOUT))
    {
        Serial.println(" : Timeout");
    }
    else
    {
        if (response.length == OKAY)
        {
            switch (request.length)
            {
            case strlen("authentication"):
                if (!sessionID)
                    recieve_new_session(&response);
                break;
            case 36U:
                if (sessionID)
                    Serial.printf("The temperature now is %.2f\n", get_the_temp(&response));
                break;
            case 37U:
                Serial.println("The led is On now");
                break;
            case 38U:
                Serial.println("The led is Off now");
                break;
            case strlen("close"):
                Serial.println("The session is closed");
            default:
                Serial.println("the response is not valid");
                break;
            }
        }
         else
         {
             err_msg(response.length);
         }
    }

    delay(500);
}
