#include <stdlib.h>
#include <bsp_io.h>
#include <common.h>
#include <rgb_driver.h>

#define GREEN_PIN 17
#define RED_PIN 18

static void turn_off_led(void);

static void (*digital_write)(uint8_t, uint8_t) = bsp_digital_write; // A function pointer for digital write function
static void (*pin_mode)(uint8_t, uint8_t) = bsp_pin_mode;           // A function pointer for pin mode function
static uint8_t rgb_led_status = UNINITIALIZED;

void rgb_driver_init(interface_t *intrfc)
{
    if (intrfc != NULL)
    {
        digital_write = intrfc->digital_write;
        pin_mode = intrfc->pin_mode;
    }

    pin_mode(GREEN_PIN, OUTPUT);
    pin_mode(RED_PIN, OUTPUT);
    digital_write(GREEN_PIN, HIGH);
    digital_write(RED_PIN, HIGH);

    rgb_led_status = OKAY;
}

uint8_t rgb_driver_green_light(void)
{
    if (rgb_led_status != UNINITIALIZED)
    {
        turn_off_led();
        digital_write(GREEN_PIN, HIGH);
        rgb_led_status = OKAY;
    }
    return rgb_led_status;
}

uint8_t rgb_driver_red_light(void)
{
    if (rgb_led_status != UNINITIALIZED)
    {
        turn_off_led();
        digital_write(RED_PIN, HIGH);
        rgb_led_status = OKAY;
    }
    return rgb_led_status;
}

uint8_t rgb_driver_yellow_light(void)
{
    if (rgb_led_status != UNINITIALIZED)
    {
        turn_off_led();
        digital_write(RED_PIN, HIGH);
        digital_write(GREEN_PIN, HIGH);
        rgb_led_status = OKAY;
    }
    return rgb_led_status;
}

/**
 * @brief This is a help function will set all the RGB led pins to LOW.
 * used before changing form one light color to another.
 */
static void turn_off_led(void)
{
    digital_write(GREEN_PIN, LOW);
    digital_write(RED_PIN, LOW);
}
