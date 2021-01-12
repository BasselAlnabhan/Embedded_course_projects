#include <RSA.h>
#include <SHA256.h>
#include <AES256.h>
#include <Arduino.h>
#include <BLEDevice.h>

#define BUFSIZE (3 * RSA_SIZE)
#define SERVER_ADDRESS "A4:CF:12:31:AD:CA"
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"           // UART Over BLE service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // Charactristic UUID for WRITE
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // Charactristic UUID for READ and NOTIFY

enum Request
{
    AUTH,
    TOKEN,
    CLOSE,
    TEMPERATURE,
    TURN_LED_ON,
    TURN_LED_OFF
};

enum Status
{
    OKAY,
    UNAUTH,
    EXPIRED,
    TIMEOUT,
    WAITING,
    HASH_ERROR,
    BAD_REQUEST,
    DISCONNECTED,
    UNKNOWN_ERROR
};

typedef struct
{
    uint64_t ID;
    uint8_t *data;
    size_t length;
} request_t;

typedef struct
{
    uint8_t *data;
    size_t length;
    uint8_t status;
} response_t;

static void printError(void);
static void sendRequest(void);
static void authenticate(void);
static bool connectToServer(void);
static void runCommand(uint8_t reqID);
static char readCommand(const char *filter);
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify);

static uint8_t server_public_key[RSA_SIZE] = {
    0x92, 0xF4, 0x13, 0xDD, 0x91, 0xFE, 0x15, 0xD3, 0xCA, 0x3D, 0xD8, 0x65, 0x32, 0x8D, 0xC7, 0x64,
    0xA5, 0x7F, 0xF9, 0x4C, 0xE2, 0x9B, 0x03, 0x96, 0xF5, 0xB7, 0x80, 0x55, 0xDE, 0xB0, 0xEA, 0x58,
    0x97, 0x7F, 0x9F, 0x25, 0x8C, 0x45, 0x5A, 0xF4, 0x50, 0x21, 0xF8, 0x95, 0xCC, 0xB8, 0x53, 0xE4,
    0x8C, 0x43, 0xD0, 0x87, 0x08, 0x97, 0xF3, 0x31, 0x39, 0x50, 0x17, 0x5F, 0xE1, 0xCD, 0xC7, 0xD5};
static uint8_t client_public_key[RSA_SIZE] = {
    0xD1, 0x13, 0x2B, 0x14, 0x8E, 0xA4, 0x70, 0x89, 0xA0, 0x3E, 0x3B, 0x2E, 0x3F, 0xDD, 0xDC, 0xC0,
    0xBD, 0x88, 0x49, 0x2B, 0xFC, 0x04, 0x6C, 0xB9, 0x23, 0x5C, 0x1F, 0x5B, 0x68, 0x6F, 0x00, 0xDD,
    0xF0, 0x8A, 0x9A, 0x12, 0x7C, 0x64, 0x33, 0x8F, 0x6B, 0xF4, 0xC5, 0x62, 0x00, 0x68, 0x20, 0xDE,
    0xD3, 0xA2, 0xA2, 0xEA, 0xD6, 0x04, 0x9C, 0x15, 0xB9, 0x23, 0xC3, 0x8E, 0x02, 0xEC, 0x7E, 0x7B};
static uint8_t client_private_key[RSA_SIZE] = {
    0x63, 0xC2, 0x98, 0xB0, 0xC3, 0x6B, 0x55, 0x43, 0x66, 0x11, 0xAB, 0x9D, 0x62, 0xDE, 0x13, 0x22,
    0x68, 0x8D, 0x6A, 0x14, 0xB1, 0xB8, 0xCE, 0xC3, 0xFC, 0x4A, 0x4D, 0xB0, 0x09, 0x01, 0xC6, 0x50,
    0x43, 0x08, 0x4B, 0xC6, 0x6D, 0x16, 0xA6, 0xA8, 0x34, 0x47, 0x46, 0xDF, 0x82, 0xDA, 0x68, 0x52,
    0xF7, 0x31, 0x38, 0xA3, 0xA5, 0xAB, 0x6C, 0x02, 0xF1, 0xA6, 0x97, 0xFF, 0x0B, 0x14, 0x65, 0x01};

static BLEClient *pClient;
static request_t request = {};
static response_t response = {};
static uint8_t buffer[BUFSIZE] = {};
static BLERemoteCharacteristic *rxCharacteristic;

static void printError(void)
{
    Serial.print("[");
    switch (response.status)
    {
    case DISCONNECTED:
        Serial.print("DISCONNECTED");
        break;

    case HASH_ERROR:
        Serial.print("HASH_ERROR");
        break;

    case BAD_REQUEST:
        Serial.print("BAD_REQUEST");
        break;

    case TIMEOUT:
        Serial.print("TIMEOUT");
        break;

    case EXPIRED:
        Serial.print("EXPIRED");
        break;

    default:
        response.status = UNAUTH;
        Serial.print("UNAUTH");
        break;
    }
    Serial.println("] An error occurred.\n");
}

static void runCommand(uint8_t reqID)
{
    request.length = 1U;
    request.data[0] = reqID;

    for (uint8_t i = 0; i < sizeof(request.ID); i++)
    {
        request.data[request.length++] = (uint8_t)(request.ID >> ((7 - i) * 8));
    }

    aes256_encrypt(request.data, request.length, request.data);
    request.length = AES_CIPHER_SIZE;

    sendRequest();

    if (response.status == OKAY)
    {
        switch (reqID)
        {
        case CLOSE:
            request.ID = 0U;
            Serial.println("Connection was closed.\n");
            break;

        case TEMPERATURE:
            if (response.length == 6)
            {
                Serial.printf("Temperature: %s\n\n", (char *)response.data);
            }
            else
            {
                Serial.println("The received temerature is invalid.\n");
            }
            break;

        case TURN_LED_OFF:
            Serial.println("The LED is turned off.\n");
            break;

        case TURN_LED_ON:
            Serial.println("The LED is turned on.\n");
            break;

        default:
            Serial.println("Unknown Request.\n");
            break;
        }
    }
    else if (response.status == UNKNOWN_ERROR)
    {
        switch (reqID)
        {
        case TURN_LED_ON:
            Serial.println("Failed to turn the LED on.\n");
            break;

        case TURN_LED_OFF:
            Serial.println("Failed to turn the LED off.\n");
            break;

        default:
            Serial.println("Unknown Request.\n");
            break;
        }
    }
    else
    {
        printError();
    }

    if (response.status == UNAUTH)
    {
        request.ID = 0U;
    }
    else if (response.status == EXPIRED)
    {
        authenticate();
    }
    else if ((response.status != TIMEOUT) && (response.status != DISCONNECTED))
    {
        if ((reqID == CLOSE) ? (response.status != OKAY) : true)
        {
            request.ID++;
            if (request.ID == 0)
            {
                request.ID++;
            }
        }
    }
}

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *data, size_t length, bool isNotify)
{
    if (isNotify && (response.status == WAITING))
    {
        if (length <= HASH_SIZE)
        {
            response.status = BAD_REQUEST;
            return;
        }

        uint8_t hash[HASH_SIZE] = {};
        sha256(data, length - HASH_SIZE, hash);
        if (memcmp(hash, data + length - HASH_SIZE, HASH_SIZE))
        {
            response.status = HASH_ERROR;
            return;
        }
        else
        {
            length -= HASH_SIZE;
        }

        if (length == 2 * RSA_SIZE)
        {
            if ((request.data[0] == AUTH) && (request.length == 2 * RSA_SIZE + 1))
            {
                uint8_t temp[RSA_SIZE] = {};
                rsa_private_decrypt(data, client_public_key, client_private_key, temp);
                rsa_private_decrypt(data + RSA_SIZE, client_public_key, client_private_key, temp + RSA_BLOCK_SIZE);
                response.length = rsa_public_decrypt(temp, server_public_key, temp);

                response.status = temp[0];
                if (response.status == OKAY)
                {
                    if (response.length == AES_KEY_SIZE + 1)
                    {
                        response.status = temp[0];
                        response.length = AES_KEY_SIZE;
                        memcpy(response.data, temp + 1, response.length);
                    }
                    else
                    {
                        response.status = BAD_REQUEST;
                    }
                }
            }
            else
            {
                response.status = BAD_REQUEST;
            }
        }
        else if ((length == AES_CIPHER_SIZE) && (request.length == AES_CIPHER_SIZE))
        {
            request.length = aes256_decrypt(data, request.data);
            if (request.length)
            {
                response.status = request.data[0];
                response.length = request.length - 1;
                memcpy(response.data, request.data + 1, response.length);
            }
            else
            {
                response.status = BAD_REQUEST;
            }
        }
        else
        {
            response.status = BAD_REQUEST;
        }
    }
}

static void sendRequest(void)
{
    response.length = 0;
    response.status = WAITING;

    if ((request.length != 2 * RSA_SIZE + 1) && (request.length != AES_CIPHER_SIZE))
    {
        Serial.println("Error");
        response.status = BAD_REQUEST;
        return;
    }

    if (!pClient->isConnected())
    {
        if (!connectToServer())
        {
            response.status = DISCONNECTED;
            return;
        }
    }

    sha256(request.data, request.length, request.data + request.length);
    rxCharacteristic->writeValue(request.data, request.length + HASH_SIZE, true);

    uint16_t elapsed = 0;
    while (elapsed < 5000)
    {
        if (response.status != WAITING)
        {
            break;
        }
        delay(1);
        elapsed++;
    }

    if (response.status == WAITING)
    {
        response.status = TIMEOUT;
    }
}

static void authenticate(void)
{
    randomSeed(micros());
    uint8_t token[sizeof(request.ID)] = {};
    for (uint8_t i = 0; i < sizeof(token); i++)
    {
        token[i] = random(0xFF);
    }

    request.ID = 0U;
    request.length = 1U;
    request.data[0] = AUTH;

    uint8_t temp[RSA_SIZE] = {};

    rsa_private_encrypt(token, sizeof(token), client_public_key, client_private_key, temp);
    rsa_public_encrypt(temp, RSA_BLOCK_SIZE, server_public_key, request.data + request.length);
    request.length += RSA_SIZE;
    rsa_public_encrypt(temp + RSA_BLOCK_SIZE, RSA_SIZE - RSA_BLOCK_SIZE, server_public_key, request.data + request.length);
    request.length += RSA_SIZE;

    sendRequest();

    if (response.status != OKAY)
    {
        return printError();
    }

    aes256_init_key(response.data);

    request.length = 1U;
    request.data[0] = TOKEN;
    memcpy(request.data + request.length, token, sizeof(token));
    aes256_encrypt(request.data, request.length + sizeof(token), request.data);
    request.length = AES_CIPHER_SIZE;

    sendRequest();

    if (response.length != sizeof(request.ID))
    {
        response.status = BAD_REQUEST;
    }

    if (response.status != OKAY)
    {
        return printError();
    }

    for (uint8_t i = 0; i < sizeof(request.ID); i++)
    {
        request.ID = (request.ID << 8U) | response.data[i];
    }
}

static bool connectToServer(void)
{
    bool status = false;

    if (pClient->connect(BLEAddress(SERVER_ADDRESS)))
    {
        BLERemoteService *pRemoteService = pClient->getService(SERVICE_UUID);
        if (pRemoteService != nullptr)
        {
            BLERemoteCharacteristic *txCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_TX);
            if (txCharacteristic != nullptr)
            {
                txCharacteristic->registerForNotify(notifyCallback);
                rxCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_RX);
                if (rxCharacteristic != nullptr)
                {
                    status = true;
                }
            }
        }
    }

    return status;
}

static char readCommand(const char *filter)
{
    char command = 0;
    bool found = false;

    while (Serial.available())
    {
        (void)Serial.read();
    }

    while (!found)
    {
        if (Serial.available())
        {
            command = toupper(Serial.read());
            for (char *ptr = (char *)filter; *ptr; ptr++)
            {
                if (*ptr == command)
                {
                    found = true;
                    break;
                }
            }
        }
    }

    Serial.printf("%c\n\n", command);

    return command;
}

void setup()
{
    Serial.begin(9600);
    delay(5000);

    BLEDevice::init("BLE_CLIENT");
    std::string address = BLEDevice::getAddress().toString();
    transform(address.begin(), address.end(), address.begin(), ::toupper);
    Serial.printf("Address: %s\n\n", address.c_str());
    pClient = BLEDevice::createClient();

    response.data = buffer;
    request.data = buffer;

    authenticate();
}

void loop()
{
    if (request.ID)
    {
        Serial.println("F) Turn The LED Off");
        Serial.println("O) Turn The LED On");
        Serial.println("T) Get Temperature");
        Serial.println("C) Close");
    }
    else
    {
        Serial.println("A) Authenticate");
    }
    Serial.print("Enter the command: ");

    char command = request.ID ? readCommand("FOTC\n") : readCommand("A\n");

    switch (command)
    {
    case 'A':
        authenticate();
        break;
    case 'T':
        runCommand(TEMPERATURE);
        break;

    case 'O':
        runCommand(TURN_LED_ON);
        break;

    case 'F':
        runCommand(TURN_LED_OFF);
        break;

    case 'C':
        runCommand(CLOSE);
        break;
    default:
        break;
    }
}