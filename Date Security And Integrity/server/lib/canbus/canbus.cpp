#include <canbus.h>
#include <FlexCAN.h>
#include <IntervalTimer.h>

#include <can_server.h>
#include <temperature.h>
#include <SHA1.h>
#include <RSA.h>
#include <AES128.h>
#include <RNGA.h>

#define TIMEOUTMS (5000U)
#define TIME_QUANTUM (15U) // ???
#define BAUD_RATE (250000U)
#define MILLI_SECOND (1000U)

static bool ready = false;
static IntervalTimer timer;
static uint8_t request_id = 0U;
static volatile bool complete = false;
static CAN_message_t tx_messages[MESSAGES] = {};
static CAN_message_t rx_messages[MESSAGES] = {};

void print_data(uint8_t *data, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if (data[i])
            Serial.printf("%02X ", data[i]);
    }
    Serial.println();
}

void send_response(response_t *res)
{
    ready = false;
    //memset(res->data + res->length, 0x55, DATA_LENGTH - res->length);
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        rx_messages[i].buf[0] = 0U;
        tx_messages[i].buf[0] = res->length;
        memcpy(tx_messages[i].buf + 1U, res->data + 7U * i, 7U);
    }
    tx_messages[MESSAGES - 1].buf[7] = request_id;
    ready = true;
}

request_t get_request(void)
{
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        memset(rx_messages[i].buf, 0, sizeof(rx_messages[i].buf));
    }

    complete = false;
    while (!complete)
    {
        delay(TIME_QUANTUM);
    }

    request_id = rx_messages[MESSAGES - 1].buf[7];
    request_t req = {};
    req.length = rx_messages[0].buf[0];
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        memcpy(req.data + 7U * i, rx_messages[i].buf + 1U, 7U);
        memset(rx_messages[i].buf, 0, sizeof(rx_messages[i].buf));
    }
    return req;
}

static void canbus_update(void)
{
    if (ready)
    {
        for (uint8_t i = 0; i < MESSAGES; i++)
        {
            Can0.write(tx_messages[i]);
        }
    }

    if (Can0.available())
    {
        CAN_message_t msg;
        if (complete)
        {
            while (Can0.read(msg))
            {
            }
        }
        else
        {
            while (Can0.read(msg))
            {
                for (uint8_t i = 0; i < MESSAGES; i++)
                {
                    if (msg.id == rx_messages[i].id)
                    {
                        rx_messages[i] = msg;
                        break;
                    }
                }
            }

            complete = true;
            for (uint8_t i = 1; i < MESSAGES; i++)
            {

                if ((rx_messages[0].buf[0] == 0U) || (rx_messages[i].buf[0] != rx_messages[0].buf[0]))
                {
                    complete = false;
                    break;
                }
            }
        }
    }
}

void canbus_begin(void)
{
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        rx_messages[i].id = i;
        tx_messages[i].len = 8U;
        tx_messages[i].id = MESSAGES + i;
    }

    Can0.begin(BAUD_RATE);

    timer.begin(canbus_update, TIME_QUANTUM * MILLI_SECOND);
}

TMP36 tmp36(tmpPin, VOLTAGE);

float temp = 0;

uint32_t sessionID = 0;

uint8_t client_hashed_id[HASH_SIZE] = {0x2e, 0xB1, 0xAC, 0x39, 0x19, 0x71, 0x15, 0x7E, 0xD5, 0x53, 0x2E, 0x85, 0x20, 0x00, 0x1B, 0xDB, 0xDC, 0x83, 0xC7, 0x0A};

uint8_t led_control_msg[] = {0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x46, 0x86, 0x52, 0x06, 0xc6, 0x56, 0x64};

uint8_t led_response_msg[] = {0x74, 0x68, 0x65, 0x20, 0x6c, 0x65, 0x64, 0x20, 0x69, 0x73, 0x20};

uint8_t temp_msg[] = {0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x65, 0x64};

uint8_t temp_response_msg[] = {0x74, 0x68, 0x65, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x20, 0x69, 0x73};

uint8_t CLIENT_ID[] = {
    0x31, 0x62, 0x61, 0x73, 0x73, 0x65, 0x6c, 0x61, 0x6c, 0x6e, 0x61, 0x62, 0x68, 0x61, 0x6e, 0x32, 0x64, 0x61, 0x6e, 0x69,
    0x65, 0x6c, 0x67, 0x72, 0x61, 0x6e, 0x65, 0x72, 0x33, 0x6d, 0x61, 0x67, 0x6e, 0x75, 0x73, 0x6b, 0x72, 0x69, 0x73, 0x74,
    0x69, 0x65, 0x6e, 0x73, 0x73, 0x6f, 0x6e};

uint8_t client_public_key[] = {
    0x8D, 0x3C, 0x18, 0xF7, 0x86, 0x90, 0x1C, 0xA9, 0xC7, 0x79, 0xB8, 0xA5, 0x3B,
    0x59, 0x0A, 0xFC, 0xB6, 0x79, 0xDA, 0x44, 0x70, 0x70, 0xCE, 0x60, 0x0B, 0xB3, 0xA7, 0x21, 0xA4, 0xBF, 0xB5, 0x81};

uint8_t server_public_key[] = {
    0x82, 0xAB, 0xAB, 0xB4, 0x24, 0x50, 0x1C, 0x61, 0x08, 0xBD, 0x99, 0xED, 0xBB, 0x09, 0xAD, 0xA3, 0x67, 0x01, 0x63,
    0xB8, 0x44, 0x87, 0x11, 0xCD, 0xA6, 0xD2, 0x31, 0x9A, 0x0D, 0x5F, 0x01, 0xE1};

uint8_t server_private_key[] = {
    0x2D, 0xDF, 0xD7, 0x08, 0x07, 0x37, 0x98, 0xBE, 0xF8, 0x32, 0x98, 0x37, 0xBD, 0xD2, 0x48, 0x02, 0xF2, 0x59, 0x5A,
    0x7F, 0xF0, 0x9F, 0x7C, 0x54, 0x09, 0x49, 0xF3, 0xDD, 0x00, 0x03, 0x07, 0x81};

static uint32_t generateRNGASessionID()
{
    rnga_init();
    rnga_seed(micros());
    uint32_t value = 0;
    while (value <= 0)
    {
        value = rnga_generate(0x0, 0xFFFFFFFF);
    }
    return value;
}

static void generate_new_session(response_t *res)
{
    // first 4 bytes are sessionID and the rest is the AESkey

    bool readyEncrypted = false;
    uint8_t aes[AES_KEY_SIZE] = {};
    if (!sessionID)
    {
        sessionID = generateRNGASessionID();
        const uint8_t *aesKey = aes128_init_key(NULL);
        memcpy(aes, aesKey, AES_KEY_SIZE);
    }

    uint8_t newSessionMsg[AES_KEY_SIZE + sizeof(sessionID)] = {};
    uint8_t finalMsg[2 * RSA_SIZE + HASH_SIZE];
    memcpy(newSessionMsg, &sessionID, sizeof(sessionID));
    memcpy(newSessionMsg + sizeof(sessionID), aes, AES_KEY_SIZE);
    uint8_t RSAHashedMsg[RSA_SIZE] = {};
    if (rsa_private_encrypt(newSessionMsg, AES_KEY_SIZE + sizeof(sessionID), server_public_key, server_private_key, RSAHashedMsg))
    {
        readyEncrypted = rsa_public_encrypt(RSAHashedMsg, RSA_BLOCK_SIZE, client_public_key, finalMsg) &&
                         rsa_public_encrypt(RSAHashedMsg + RSA_BLOCK_SIZE, RSA_SIZE - RSA_BLOCK_SIZE, client_public_key, finalMsg + RSA_SIZE);
    }
    if (readyEncrypted)
    {
        sha1(finalMsg, 2 * RSA_SIZE, finalMsg + 2 * RSA_SIZE);
        memcpy(res->data, finalMsg, 2 * RSA_SIZE + HASH_SIZE);
        // res->length = (2 * RSA_SIZE + HASH_SIZE);
    }

    // Serial.printf("sessionID %d, AESkey:", sessionID);
    // print_data(aes, 16U);
}

response_t authenticate(request_t *req)
{
    response_t response = {};
    uint8_t hashedInMsg[HASH_SIZE] = {}, decryptedBuffer[2 * RSA_SIZE] = {}, data[2 * RSA_SIZE] = {};
    sha1(req->data, 2 * RSA_SIZE, hashedInMsg);

    uint8_t *hashptr = req->data + (2 * RSA_SIZE);
    if (!memcmp(hashedInMsg, hashptr, HASH_SIZE))
    {

        uint8_t priDecryptionBytes = rsa_private_decrypt(req->data, server_public_key, server_private_key, decryptedBuffer);
        priDecryptionBytes += rsa_private_decrypt(req->data + RSA_SIZE, server_public_key, server_private_key, decryptedBuffer + priDecryptionBytes);

        uint8_t pubDecryptedBytes = rsa_public_decrypt(decryptedBuffer, client_public_key, data);

        if (!memcmp(data, client_hashed_id, HASH_SIZE))
        {
            generate_new_session(&response);
            response.length = OKAY;
        }
        else if (sessionID)
        {
            response.length = OKAY;
        }
        else if (!sessionID)
            response.length = UNAUTH;
    }
    else
    {
        response.length = HASH_ERROR;
    }

    return response;
}

response_t get_temp(request_t *req)
{

    response_t res = {.length = UNAUTH};

    uint8_t hash[HASH_SIZE] = {}, encrypted[AES_CIPHER_SIZE + HASH_SIZE] = {};
    sha1(req->data, AES_CIPHER_SIZE, hash);
    uint8_t *hashptr = req->data + AES_CIPHER_SIZE;

    if (!memcmp(hash, hashptr, HASH_SIZE) && !temp)
    {
        tmp36.update();
        temp = tmp36.getCelsius();
        memcpy(encrypted, temp_response_msg, sizeof(temp_response_msg));
        memcpy(encrypted + sizeof(temp_response_msg), &temp, sizeof(temp));

        if (aes128_encrypt(encrypted, sizeof(temp_response_msg) + sizeof(temp), encrypted))
        {

            sha1(encrypted, AES_CIPHER_SIZE, encrypted + AES_CIPHER_SIZE);

            memcpy(res.data, encrypted, sizeof(encrypted));
            res.length = OKAY;
            Serial.println(temp);
        }
    }
    else
    {
        res.length = HASH_ERROR;
    }
    if (!sessionID)
    {
        res.length = EXPIRED;
    }
    return res;
}