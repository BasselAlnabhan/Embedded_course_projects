#include <RSA.h>
#include <SHA256.h>
#include <AES256.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <esp32-hal.h>

#define BUFSIZE (3 * RSA_SIZE)
#define CLIENT_ADDRESS "A4:CF:12:78:59:4A"
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
    size_t length;
    uint8_t data[BUFSIZE];
} response_t;

static uint8_t client_public_key[RSA_SIZE] = {
    0xD1, 0x13, 0x2B, 0x14, 0x8E, 0xA4, 0x70, 0x89, 0xA0, 0x3E, 0x3B, 0x2E, 0x3F, 0xDD, 0xDC, 0xC0,
    0xBD, 0x88, 0x49, 0x2B, 0xFC, 0x04, 0x6C, 0xB9, 0x23, 0x5C, 0x1F, 0x5B, 0x68, 0x6F, 0x00, 0xDD,
    0xF0, 0x8A, 0x9A, 0x12, 0x7C, 0x64, 0x33, 0x8F, 0x6B, 0xF4, 0xC5, 0x62, 0x00, 0x68, 0x20, 0xDE,
    0xD3, 0xA2, 0xA2, 0xEA, 0xD6, 0x04, 0x9C, 0x15, 0xB9, 0x23, 0xC3, 0x8E, 0x02, 0xEC, 0x7E, 0x7B};
static uint8_t server_public_key[RSA_SIZE] = {
    0x92, 0xF4, 0x13, 0xDD, 0x91, 0xFE, 0x15, 0xD3, 0xCA, 0x3D, 0xD8, 0x65, 0x32, 0x8D, 0xC7, 0x64,
    0xA5, 0x7F, 0xF9, 0x4C, 0xE2, 0x9B, 0x03, 0x96, 0xF5, 0xB7, 0x80, 0x55, 0xDE, 0xB0, 0xEA, 0x58,
    0x97, 0x7F, 0x9F, 0x25, 0x8C, 0x45, 0x5A, 0xF4, 0x50, 0x21, 0xF8, 0x95, 0xCC, 0xB8, 0x53, 0xE4,
    0x8C, 0x43, 0xD0, 0x87, 0x08, 0x97, 0xF3, 0x31, 0x39, 0x50, 0x17, 0x5F, 0xE1, 0xCD, 0xC7, 0xD5};
static uint8_t server_private_key[RSA_SIZE] = {
    0x29, 0xD2, 0xC5, 0x84, 0x9D, 0xF4, 0x4E, 0x8A, 0x04, 0x59, 0x2D, 0xA9, 0x3F, 0x86, 0x12, 0x65,
    0x96, 0xA4, 0xA1, 0x73, 0x3C, 0x5B, 0x19, 0xDE, 0x70, 0xF8, 0x54, 0xD2, 0x1B, 0x9B, 0x06, 0x56,
    0xD7, 0x67, 0x93, 0x1A, 0x0B, 0x95, 0x06, 0xD1, 0xF2, 0x04, 0x64, 0xCB, 0x13, 0xDA, 0x9E, 0x25,
    0x18, 0xCA, 0x79, 0xAB, 0x29, 0xCF, 0xE4, 0x02, 0xBA, 0x3E, 0x3B, 0xDC, 0x53, 0x84, 0x43, 0x51};

static response_t response = {};
static uint32_t lastAccess = 0U;
static BLECharacteristic *txCharacteristic;
static BLEAddress clientAddress = BLEAddress(CLIENT_ADDRESS);

static void sendResponse(void)
{
    sha256(response.data, response.length, response.data + response.length);
    txCharacteristic->setValue(response.data, response.length + HASH_SIZE);
    txCharacteristic->notify();
}

static void encryptResponse(bool rsa)
{
    if (rsa)
    {
        uint8_t buffer[RSA_SIZE] = {};
        rsa_private_encrypt(response.data, response.length, server_public_key, server_private_key, buffer);
        rsa_public_encrypt(buffer, RSA_BLOCK_SIZE, client_public_key, response.data);
        rsa_public_encrypt(buffer + RSA_BLOCK_SIZE, RSA_SIZE - RSA_BLOCK_SIZE, client_public_key, response.data + RSA_SIZE);
        response.length = 2 * RSA_SIZE;
    }
    else
    {
        aes256_encrypt(response.data, response.length, response.data);
        response.length = AES_CIPHER_SIZE;
    }
}

static void buildResponse(uint8_t *request, uint8_t length)
{
    static uint8_t token[sizeof(response.ID)] = {};

    if (length % 2)
    {
        if ((request[0] == AUTH) && (length == 2 * RSA_SIZE + 1))
        {
            uint8_t buffer[RSA_SIZE] = {};
            rsa_private_decrypt(request + 1, server_public_key, server_private_key, buffer);
            rsa_private_decrypt(request + RSA_SIZE + 1, server_public_key, server_private_key, buffer + RSA_BLOCK_SIZE);
            rsa_public_decrypt(buffer, client_public_key, buffer);
            memcpy(token, buffer, sizeof(token));

            response.length = 1U;
            response.data[0] = OKAY;
            const uint8_t *key = aes256_init_key(NULL);
            memcpy(response.data + response.length, key, AES_KEY_SIZE);
            response.length += AES_KEY_SIZE;
        }
        else
        {
            response.length = 1U;
            response.data[0] = BAD_REQUEST;
        }

        return encryptResponse(true);
    }

    if (length != AES_CIPHER_SIZE)
    {
        response.length = 1U;
        response.data[0] = BAD_REQUEST;
        return encryptResponse(false);
    }

    if ((sizeof(token) + 1) != aes256_decrypt(request, request))
    {
        response.length = 1U;
        response.data[0] = BAD_REQUEST;
        return encryptResponse(false);
    }

    if (request[0] == TOKEN)
    {
        if (memcmp(request + 1, token, sizeof(token)))
        {
            response.length = 1U;
            response.data[0] = UNAUTH;
        }
        else
        {
            randomSeed(micros());
            uint8_t responseID[sizeof(response.ID)] = {};
            for (uint8_t i = 0; i < sizeof(responseID); i++)
            {
                responseID[i] = random(0x01, 0xFF);
                response.ID = (response.ID << 8U) | responseID[i];
            }

            response.length = 1U;
            response.data[0] = OKAY;
            memcpy(response.data + response.length, responseID, sizeof(responseID));
            response.length += sizeof(responseID);
            lastAccess = millis();
        }
    }
    else
    {
        uint64_t requestID = 0U;
        for (uint8_t i = 1; i <= sizeof(requestID); i++)
        {
            requestID = (requestID << 8U) | request[i];
        }

        if (requestID != response.ID)
        {
            response.length = 1U;
            response.data[0] = UNAUTH;
            return encryptResponse(false);
        }

        response.ID++;
        if (response.ID == 0)
        {
            response.ID++;
        }

        uint32_t elapsed = 0;
        uint32_t millisec = millis();

        if (lastAccess > millisec)
        {
            elapsed = 0xFFFFFFFF - lastAccess + millisec + 1;
        }
        else
        {
            elapsed = millisec - lastAccess;
        }
        lastAccess = millisec;

        if (elapsed > 60000)
        {
            response.ID = 0U;
            response.length = 1U;
            response.data[0] = EXPIRED;
        }
        else if (request[0] == CLOSE)
        {
            response.ID = 0U;
            response.length = 1U;
            response.data[0] = OKAY;
        }
        else if (request[0] == TURN_LED_OFF)
        {
            response.length = 1U;
            digitalWrite(BUILTIN_LED, LOW);
            response.data[0] = (digitalRead(BUILTIN_LED) == LOW) ? OKAY : UNKNOWN_ERROR;
        }
        else if (request[0] == TURN_LED_ON)
        {
            response.length = 1U;
            digitalWrite(BUILTIN_LED, HIGH);
            response.data[0] = (digitalRead(BUILTIN_LED) == HIGH) ? OKAY : UNKNOWN_ERROR;
        }
        else if (request[0] == TEMPERATURE)
        {
            response.length = 1U;
            response.data[0] = OKAY;

            char temp[6] = {};
            sprintf(temp, "%.02f", temperatureRead());
            memcpy(response.data + response.length, temp, sizeof(temp));
            response.length += sizeof(temp);
        }
        else
        {
            response.length = 1U;
            response.data[0] = BAD_REQUEST;
        }
    }

    encryptResponse(false);
}

class CharactristicCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();
        uint8_t *data = (uint8_t *)rxValue.c_str();
        uint8_t length = rxValue.length();

        response.length = 0U;

        if (length <= HASH_SIZE)
        {
            response.length = 1U;
            response.data[0] = BAD_REQUEST;
        }
        else
        {
            uint8_t hash[HASH_SIZE] = {};
            sha256(data, length - HASH_SIZE, hash);
            if (memcmp(hash, data + length - HASH_SIZE, HASH_SIZE))
            {
                response.length = 1U;
                response.data[0] = HASH_ERROR;
            }
            else
            {
                length -= HASH_SIZE;
            }
        }

        if (!response.length)
        {
            buildResponse(data, length);
        }
        else if (length % 2)
        {
            encryptResponse(true);
        }
        else
        {
            encryptResponse(false);
        }

        sendResponse();
    }
};

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
    }
};

void setup()
{
    aes256_init_key(NULL);

    Serial.begin(9600);
    delay(5000);

    pinMode(LED_BUILTIN, OUTPUT);

    // Create the BLE Device
    BLEDevice::init("BLE_SERVER");

    std::string address = BLEDevice::getAddress().toString();
    transform(address.begin(), address.end(), address.begin(), ::toupper);
    Serial.printf("Address: %s\n", address.c_str());

    BLEDevice::whiteListAdd(clientAddress);

    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic for TX
    txCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    txCharacteristic->addDescriptor(new BLE2902());

    // Create a BLE Characteristic for RX
    BLECharacteristic *rxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    rxCharacteristic->addDescriptor(new BLE2902());

    rxCharacteristic->setCallbacks(new CharactristicCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanFilter(true, true);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Waiting on a client connection to notify...");
}

void loop()
{
}