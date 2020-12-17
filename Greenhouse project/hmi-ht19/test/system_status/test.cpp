#ifdef TARGET
#include <Arduino.h>
#endif

#include <unity.h>
#include <bsp_io.h>
#include <common.h>
#include <canbus.h>
#include <rgb_driver.h>
#include <buzzer_driver.h>
#include <system_status.h>

static int16_t state;                 // This variable is used to test if the number of written values to the pins are correct and in order
static uint8_t outpins;               // Number of pins in OUTPUT mode
static uint16_t n_times;              // Number of digital writes
static interface_t *interface = NULL; // A pointer to the interface. NULL means that we will use the default functions which are pinMode, delay and digitalWrite

void buzzer_driver_init(void) {}

void buzzer_driver_tone(uint16_t frequency, uint32_t duration) {}

void buzzer_driver_no_tone(void) {}

static void reset_variables(void)
{
    state = 0;
    outpins = 0;
    n_times = 0;
}

#ifdef TARGET
void digital_write_spy(uint8_t pin, uint8_t val)
{
    n_times++; // Number of calls to digitalWrite
    if (val == HIGH)
    {
        state++; // If we set the pin to HIGH, state is incremented
    }
    else
    {
        state--; // If we set the pin to LOW, state is decremented
    }
    bsp_digital_write(pin, val); // bsp_digital_write on Teensy
}

void pin_mode_spy(uint8_t pin, uint8_t mode)
{
    outpins++;               // If the mode is correct, increment outpins
    bsp_pin_mode(pin, mode); // bsp_pin_mode on Teensy
}

#else
void bsp_digital_write(uint8_t pin, uint8_t val)
{
    n_times++;
    if (val == HIGH)
    {
        state++;
    }
    else
    {
        state--;
    }
}

void bsp_pin_mode(uint8_t pin, uint8_t mode)
{
    outpins++;
}

#endif

void setUp()
{
    reset_variables();
    system_status_init();
}

void tearDown()
{
}

void test_status_okay()
{
    set_test_esp32_status(OKAY);

    set_test_water_pump_status(OKAY);

    set_test_water_valve_status(OKAY);

    set_test_flow_meter_sensor_status(OKAY);

    set_test_rtc_status(OKAY);

    set_test_terminal_status(OKAY);

    set_test_light_intensity_sensor_status(OKAY);

    set_test_soil_moisture_sensor_status(OKAY);

    set_test_water_level_sensor_status(OKAY);

    set_test_dht_sensor_status(OKAY); //THIS IS TEMPERATURE AND HUMIDITY COMBINED.

    set_test_sdcard_status(OKAY);

    set_test_window_status(OKAY);

    set_test_lamp_status(OKAY);

    set_test_heater_status(OKAY);

    set_test_fans_status(OKAY);

    set_keypad_status(OKAY);

    set_rgb_status(OKAY);

    set_display_status(OKAY);

    set_eeprom_status(OKAY);

    set_buzzer_status(OKAY);

    system_status_run();
    TEST_ASSERT_EQUAL_UINT8(OKAY, get_system_status());
}

void test_status_warning()
{
    set_buzzer_status(UNINITIALIZED);
    system_status_run();
    TEST_ASSERT_EQUAL_UINT8(WARNING, get_system_status());
}

void test_status_error1()
{
    set_buzzer_status(ERROR);
    system_status_run();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_system_status());
}

void test_status_error2()
{
    set_test_esp32_status(WIFI_DISCONNECTED);
    system_status_run();
    TEST_ASSERT_EQUAL_UINT8(ERROR, get_system_status());
}

#ifdef TARGET
void loop()
{
}

void setup()
{
    /**
     * @brief On Teensy we provide our implementation for digitalWrite and pinMode functions
     *        and we assign these functions to the function pointers of the interface
     */
    interface_t temp = {};
    temp.digital_write = digital_write_spy;
    temp.pin_mode = pin_mode_spy;
    interface = &temp;
    delay(3000U);

#else
int main(void)
{
#endif
    UNITY_BEGIN();
    RUN_TEST(test_status_okay);
    RUN_TEST(test_status_warning);
    RUN_TEST(test_status_error1);
    RUN_TEST(test_status_error2);

#ifdef TARGET
    UNITY_END();
#else
    return UNITY_END();
#endif
}