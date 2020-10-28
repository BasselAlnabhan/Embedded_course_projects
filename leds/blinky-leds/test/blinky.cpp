#include <unity.h>
#include <blinky.h>
#include <Arduino.h>

static int16_t state;                 // This variable is used to test if the number of written values to the pins are correct and in order
static uint8_t outpins;               // Number of pins in OUTPUT mode
static uint16_t n_times;              // Number of digital writes
static uint32_t millisecs;            // This variable is used to test the delays
static interface_t *interface = NULL; // A pointer to the interface. NULL means that we will use the default functions which are pinMode, delay and digitalWrite

static void reset_variables(void)
{
    state = 0;
    outpins = 0;
    n_times = 0;
    millisecs = 0;
}

#ifdef TEENSY
void digitalWrite_spy(uint8_t pin, uint8_t val)
{
    n_times++; // Number of calls to digitalWrite
    if (val == HIGH)
    {
        if (state >= 0)
        {
            state++; // If we set the pin to HIGH, state is increamented
        }
    }
    else
    {
        state--; // If we set the pin to LOW, state is decreamented
    }
    digitalWrite(pin, val); // digitalWrie of Arduino on Teensy
}

void pinMode_spy(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        outpins++;          // If the mode is correct, increament outpins
        pinMode(pin, mode); // pinMode of Arduino on Teensy
    }
}

void delay_spy(uint32_t msec)
{
    millisecs += msec; // Sum of the delays we use in order to make the leds blink
    delay(msec);       // delay of Arduino on Teensy
}
#else
void digitalWrite(uint8_t pin, uint8_t val)
{
    n_times++;
    if (val == HIGH)
    {
        if (state >= 0)
        {
            state++;
        }
    }
    else
    {
        state--;
    }
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        outpins++;
    }
}

void delay(uint32_t msec)
{
    millisecs += msec;
}
#endif

void setUp()
{
    reset_variables();
}

void tearDown()
{
    blinky_end();
}

void test_begin_blinky_leds()
{
    uint8_t leds[MAX_LEDS + 1] = {};
    for (uint8_t i = 0; i < sizeof(leds); i++)
    {
        leds[i] = i;
    }

    /* Test for one led */
    reset_variables();
    TEST_ASSERT_EQUAL_UINT8(BLINKY_SUCCESS, blinky_begin(interface, leds, MIN_LEDS));
    TEST_ASSERT_EQUAL_UINT8(outpins, MIN_LEDS);
    TEST_ASSERT_EQUAL_INT16(-MIN_LEDS, state);

    /* Test for multiple leds */
    reset_variables();
    TEST_ASSERT_EQUAL_UINT8(BLINKY_SUCCESS, blinky_begin(interface, leds, MAX_LEDS));
    TEST_ASSERT_EQUAL_UINT8(outpins, MAX_LEDS); // If the mode of all the pins has been set to OUTPUT
    TEST_ASSERT_EQUAL_INT16(-MAX_LEDS, state);  // If the state of all the pins has been set to LOW

    /* Test for duplicate pins */
    leds[1] = 0;
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, leds, MAX_LEDS));
    leds[1] = 1;

    /* Test for invalid arguments */
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, NULL, MIN_LEDS));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, NULL, MAX_LEDS));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, NULL, MIN_LEDS - 1));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, NULL, MAX_LEDS + 1));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, leds, MIN_LEDS - 1));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_begin(interface, leds, MAX_LEDS + 1));
}

void test_blinky_blink_times()
{
    uint8_t times = 1;
    uint16_t ms_delay = 500;
    uint8_t leds[] = {1, 2, 3, 13};

    /* Test if the module has been initialized */
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_blink_times(times, ms_delay));

    /* Test the module initialization */
    reset_variables();
    TEST_ASSERT_EQUAL_UINT8(BLINKY_SUCCESS, blinky_begin(interface, leds, sizeof(leds)));
    TEST_ASSERT_EQUAL_UINT8(outpins, sizeof(leds));
    TEST_ASSERT_EQUAL_INT16(-sizeof(leds), state);

    /* Test for one blink */
    reset_variables();
    TEST_ASSERT_EQUAL_UINT8(BLINKY_SUCCESS, blinky_blink_times(times, ms_delay));
    TEST_ASSERT_EQUAL_UINT8(times, n_times / (2 * sizeof(leds))); // For each led we change the state of the led two times
    TEST_ASSERT_EQUAL_UINT8(ms_delay, millisecs / (2 * times));   // For each led we have two delays
    TEST_ASSERT_EQUAL_INT16(0, state);

    /* Test for multiple blinks */
    times = 10;
    ms_delay = 300;
    reset_variables();
    TEST_ASSERT_EQUAL_UINT8(BLINKY_SUCCESS, blinky_blink_times(times, ms_delay));
    TEST_ASSERT_EQUAL_UINT8(times, n_times / (2 * sizeof(leds)));
    TEST_ASSERT_EQUAL_UINT8(ms_delay, millisecs / (2 * times));
    TEST_ASSERT_EQUAL_INT16(0, state);

    /* Test invalid arguments */
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_blink_times(0, ms_delay));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_blink_times(times, MIN_BLINK_DELAY - 1));
    TEST_ASSERT_EQUAL_UINT8(BLINKY_ERROR, blinky_blink_times(times, MAX_BLINK_DELAY + 1));
}

#ifdef TEENSY
void loop()
{
}

void setup()
{
    /**
     * @brief On Teensy we provide our implementation for delay, digitalWrite and pinMode functions
     *        and we assign these functions to the function pointers of the interface
     */
    interface_t temp = {};
    temp.delay_msec = delay_spy;
    temp.digital_write = digitalWrite_spy;
    temp.pin_mode = pinMode_spy;
    interface = &temp;

    delay(3000U);
#else
int main(void)
{
#endif
    UNITY_BEGIN();

    RUN_TEST(test_begin_blinky_leds);
    RUN_TEST(test_blinky_blink_times);

#ifdef TEENSY
    UNITY_END();
#else
    return UNITY_END();
#endif
}