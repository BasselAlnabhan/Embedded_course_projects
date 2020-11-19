#include <FlexCAN.h>

#define MESSAGES 3        // Number of the messages
#define TX_TIME 1000      // Transmission cycle time in ms
#define RX_TIME 100       // Reception cycle time in ms
#define MILLI_SECOND 1000 // One millisecond
#define BAUD_RATE 250000  // CAN speed

static IntervalTimer timer;
static int16_t rx_timer = RX_TIME;
static int16_t tx_timer = TX_TIME;
static CAN_message_t messages[MESSAGES] = {
    {.id = 0x090, .timestamp = 0, .flags = {.extended = 0, .remote = 0, .overrun = 0, .reserved = 0}, .len = 8, .buf = {0}},
    {.id = 0x100, .timestamp = 0, .flags = {.extended = 0, .remote = 0, .overrun = 0, .reserved = 0}, .len = 8, .buf = {0}},
    {.id = 0x110, .timestamp = 0, .flags = {.extended = 0, .remote = 0, .overrun = 0, .reserved = 0}, .len = 8, .buf = {0}},
};

static void timing(void)
{
    if (tx_timer > 0)
    {
        --tx_timer;
    }

    if (rx_timer > 0)
    {
        --rx_timer;
    }
}

void setup()
{
    Serial.begin(9600);
    delay(2000);

    // Fill the buffers
    memcpy(messages[0].buf, "ABCDEFGH", messages[0].len);
    memcpy(messages[1].buf, "IJKLMNOP", messages[1].len);
    memcpy(messages[2].buf, "QRSTUVWX", messages[2].len);

    Serial.println("\nStarting Can0 in loopback mode ...");
    Can0.loopBack = true;
    Can0.begin(BAUD_RATE);

    Serial.println("Setting the filters...");

    // Set up a blockin filter/mask for all the RX mailboxes
    CAN_filter_t filter = {.id = 0x7FF, .flags = {.extended = 1, .remote = 0, .reserved = 0}}; // The blockin filter

    for (int i = 0; i < NUM_MAILBOXES; ++i)
    {
        Can0.setFilter(filter, i);
        Can0.setMask(0x000, i);
    }

    //Set up the filter/mask for the first mailbox in order to let the first message in
    filter.id = 0x100;
    filter.flags.extended = 0;
    Can0.setFilter(filter, 0);
    Can0.setMask(0x080 << 18, 0); // 0x100, 0x110; 3 msb bits are reserved

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("Starting timer...\n");
    timer.begin(timing, MILLI_SECOND);
}

void loop()
{
    if (rx_timer == 0)
    {
        if (Can0.available())
        {
            CAN_message_t message;
            while (Can0.read(message))
            {
                Serial.printf("[%d] RX - ID: 0x%03X - [%d] - ", message.timestamp, message.id, message.len);
                for (int i = 0; i < message.len; ++i)
                {
                    Serial.printf("%c ", message.buf[i]);
                }
                Serial.printf("\n");
            }
            Serial.printf("\n");
        }

        digitalWrite(LED_BUILTIN, HIGH);
        rx_timer = RX_TIME; //Reset rx_timer
    }

    if (tx_timer == 0)
    {
        for (uint8_t i = 0; i < MESSAGES; i++)
        {
            Can0.write(messages[i]);
        }

        digitalWrite(LED_BUILTIN, LOW);
        tx_timer = TX_TIME; //Reset tx_timer
    }
}