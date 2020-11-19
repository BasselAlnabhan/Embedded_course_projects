#include <canbus.h>
#include <FlexCAN.h>
#include <IntervalTimer.h>

#include <canbus.h>
#include <client.h>
#include <RSA.h>
#include <AES128.h>
#include <SHA1.h>

#define TIMEOUTMS (5000U)
#define TIME_QUANTUM (15U) // ???
#define BAUD_RATE (250000U)
#define MILLI_SECOND (1000U)

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

response_t send_request(request_t *req)
{
    complete = true;
    // memset(req->data + req->length, 0x55, DATA_LENGTH - req->length);
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        rx_messages[i].buf[0] = 0U;
        tx_messages[i].buf[0] = req->length;
        memcpy(tx_messages[i].buf + 1U, req->data + 7U * i, 7U);
    }
    tx_messages[MESSAGES - 1].buf[7] = request_id;
    complete = false;

    response_t res = {};
    uint16_t elapsed = 0U;
    while (elapsed < TIMEOUTMS)
    {
        if (complete)
        {
            break;
        }
        delay(TIME_QUANTUM);
        elapsed += TIME_QUANTUM;
    }

    request_id++;
    if (complete)
    {
        res.length = rx_messages[0].buf[0];
        rx_messages[MESSAGES - 1].buf[7] = 0U;
        for (uint8_t i = 0; i < MESSAGES; i++)
        {
            memcpy(res.data + 7U * i, rx_messages[i].buf + 1U, 7U);
        }
    }
    else
    {
        res.length = 1U;
        res.data[0] = TIMEOUT;
    }

    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        memset(rx_messages[i].buf, 0, sizeof(rx_messages[i].buf));
    }
    complete = true;

    return res;
}

static void canbus_update(void)
{
    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        Can0.write(tx_messages[i]);
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

            if (request_id == rx_messages[MESSAGES - 1].buf[7])
            {
                complete = true;
                for (uint8_t i = 1; i < MESSAGES; i++)
                {
                    if ((rx_messages[0].buf[0] == 0U) || (rx_messages[i].buf[0] != rx_messages[0].buf[0]))
                    {
                        complete = false;
                        break;
                    }
                }
                request_id++;
            }
        }
    }
}

void canbus_begin(void)
{
    request_id = random(0xFF);

    for (uint8_t i = 0; i < MESSAGES; i++)
    {
        tx_messages[i].id = i;
        tx_messages[i].len = 8U;
        rx_messages[i].id = MESSAGES + i;
    }

    Can0.begin(BAUD_RATE);

    timer.begin(canbus_update, TIME_QUANTUM * MILLI_SECOND);
}

// Process variables
uint8_t led_on_msg[] = {0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x46, 0x86, 0x52, 0x06, 0xc6, 0x56, 0x64, 0x01};

uint8_t led_off_msg[] = {0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x46, 0x86, 0x52, 0x06, 0xc6, 0x56, 0x64, 0x00};

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

uint8_t client_private_key[] = {
    0x0D, 0x11, 0x72, 0x7D, 0xF4, 0xC5, 0x80, 0xE1, 0x74, 0xC3, 0x9E, 0x82, 0x2B,
    0x6B, 0xA1, 0xC8, 0x7D, 0x1C, 0xBF, 0xC7, 0x0E, 0xFB, 0x1A, 0xC7, 0xED, 0x70, 0x31, 0x9E, 0xF7, 0xE8, 0xDA, 0xC1};

uint8_t server_public_key[] = {
    0x82, 0xAB, 0xAB, 0xB4, 0x24, 0x50, 0x1C, 0x61, 0x08, 0xBD, 0x99, 0xED, 0xBB, 0x09, 0xAD, 0xA3, 0x67, 0x01, 0x63,
    0xB8, 0x44, 0x87, 0x11, 0xCD, 0xA6, 0xD2, 0x31, 0x9A, 0x0D, 0x5F, 0x01, 0xE1};

uint8_t aes_key[AES_KEY_SIZE] = {};

uint32_t sessionID = 0;

// Process functions
static request_t login_request(void)
{
    request_t req;
    uint8_t passLength = 0,
            passIndexInClientID[PASSWORD_LENGTH] = {0, 1, 2, 3, 5, 8, 13, 21},
            RSAhashedId[RSA_SIZE] = {},
            hashedID[HASH_SIZE] = {};
    char password[PASSWORD_LENGTH] = {};
    bool encrypted = false;

    //	getting the password from the user
    Serial.println("Enter your password please [8 charachters]: ");
    uint8_t pass_char = 'x';
    while (pass_char != '\n')
    {
        pass_char = Serial.read();
        if (pass_char == '\r')
            pass_char = Serial.read();
        else if (isalnum(pass_char))
        {
            Serial.printf("*", pass_char);
            password[passLength++] = pass_char;
        }
    }
    Serial.println();

    // Embedd the password in clientID
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
    {
        CLIENT_ID[passIndexInClientID[i]] = password[i];
    }

    //	Hashing the password with SHA1
    sha1(CLIENT_ID, sizeof(CLIENT_ID), hashedID);
    // making the signature
    if (rsa_private_encrypt(hashedID, HASH_SIZE, client_public_key, client_private_key, RSAhashedId))
    {

        // encrypt with the server public key
        encrypted = rsa_public_encrypt(RSAhashedId, RSA_BLOCK_SIZE, server_public_key, req.data) &&
                    rsa_public_encrypt(RSAhashedId + RSA_BLOCK_SIZE, RSA_SIZE - RSA_BLOCK_SIZE, server_public_key, req.data + RSA_SIZE);
        if (encrypted)
        {
            // hash the final msg
            sha1(req.data, 2 * RSA_SIZE, req.data + (2 * RSA_SIZE));
        }
    }
    req.length = strlen("authentication");
    return req;
}

static request_t close_the_session(void)
{
    request_t close;
    sessionID = 0;
    close.length = strlen("close");
    return close;
}

float get_the_temp(response_t *res)
{
    float temp = 0;
    uint8_t hash_from_rx[HASH_SIZE] = {}, data[84U] = {};

    sha1(res->data, AES_CIPHER_SIZE, hash_from_rx);
    if (!memcmp(res->data + AES_CIPHER_SIZE, hash_from_rx, HASH_SIZE))
    {
        uint8_t decrypted_bytes = aes128_decrypt(res->data, data);
        decrypted_bytes += aes128_decrypt(res->data + AES_CIPHER_SIZE, data + decrypted_bytes);
        if (!memcmp(temp_response_msg, data, sizeof(temp_response_msg)))
        {
            memcpy(&temp, data + sizeof(temp_response_msg), sizeof(float));
        }
    }
    return temp;
}

static request_t ask_for_temp(void)
{
    uint8_t aes_temp_msg[AES_CIPHER_SIZE + HASH_SIZE] = {};
    request_t temp_req = {.length = 36U};

    if (aes128_encrypt(temp_msg, sizeof(temp_msg), aes_temp_msg))
    {
        sha1(aes_temp_msg, AES_KEY_SIZE, aes_temp_msg + AES_KEY_SIZE);
        memcpy(temp_req.data, aes_temp_msg, sizeof(aes_temp_msg));
    }
    else
    {
        Serial.println("askfortemp___the encryption faild");
    }
    return temp_req;
}

static request_t turn_on_led(void)
{
    uint8_t aes_led_msg[AES_CIPHER_SIZE + HASH_SIZE] = {};
    request_t led_on = {.length = 37U};

    if (aes128_encrypt(led_on_msg, sizeof(led_on_msg), aes_led_msg))
    {
        sha1(aes_led_msg, AES_KEY_SIZE, aes_led_msg + AES_KEY_SIZE);
        memcpy(led_on.data, aes_led_msg, sizeof(aes_led_msg));
    }
    else
    {
        Serial.println("trun on the led___the encryption faild");
    }
    return led_on;
}

static request_t turn_off_led(void)
{
    uint8_t aes_led_msg[AES_CIPHER_SIZE + HASH_SIZE] = {};
    request_t led_off = {.length = 38U};

    if (aes128_encrypt(led_off_msg, sizeof(led_off_msg), aes_led_msg))
    {
        sha1(aes_led_msg, AES_KEY_SIZE, aes_led_msg + AES_KEY_SIZE);
        memcpy(led_off.data, aes_led_msg, sizeof(aes_led_msg));
    }
    else
    {
        Serial.println("trun off the led___the encryption faild");
    }
    return led_off;
}

request_t get_user_request(void)
{

    Serial.println("\n\t*** Welcome to BaDaMa project ***\n");
    Serial.printf("Enter:\nA. Authenticate\nC. Close the session\nN. Turn on the LED\nF. Turn off the LED\nT. Get the temperature\nYour choice: ");
    char input = 'x';
    while (input != 'A' && input != 'C' && input != 'N' && input != 'F' && input != 'T')
    {
        input = toupper(Serial.read());

        if (input == 'A' || input == 'C' || input == 'T' || input == 'N' || input == 'F')
        {
            Serial.printf("%c\n", input);
        }
    }

    // process the user choice
    switch (input)
    {
    case 'A':
        if (!sessionID)
            return login_request();
        else
            Serial.println("Already authenticated");
        break;
    case 'C':
        if (sessionID)
            return close_the_session();
        break;
    case 'N':
        if (sessionID)
            return turn_on_led();
        break;
    case 'F':
        if (sessionID)
            return turn_off_led();
        break;
    case 'T':
        if (sessionID)
            return ask_for_temp();
        else
            Serial.println("You need to loog in first!");
        break;
    }
    request_t request = {};
    return request;
}

void recieve_new_session(response_t *res)
{
    uint8_t data[AES_CIPHER_SIZE + sizeof(sessionID)] = {}, hashedInMsg[HASH_SIZE] = {}, decryptedBuffer[2 * RSA_SIZE] = {};
    uint8_t *hashptr = res->data + (2 * RSA_SIZE);

    sha1(res->data, 2 * RSA_SIZE, hashedInMsg);

    if (!memcmp(hashedInMsg, hashptr, HASH_SIZE))
    {
        uint8_t priDecryptionBytes = rsa_private_decrypt(res->data, client_public_key, client_private_key, decryptedBuffer);
        priDecryptionBytes += rsa_private_decrypt(res->data + RSA_SIZE, client_public_key, client_private_key, decryptedBuffer + priDecryptionBytes);

        uint8_t pubDecryptedBytes = rsa_public_decrypt(decryptedBuffer, server_public_key, data);

        memcpy(&sessionID, data, sizeof(sessionID));
        memcpy(aes_key, data + sizeof(sessionID), sizeof(aes_key));
        aes128_init_key(aes_key);
    }
    else
    {
        Serial.println("Hash error in recieving new session");
    }
}