#include <bsp.h>
#include <common.h>
#include <ambient.h>
#include <watering.h>
#include <bsp_time.h>
#include <scheduler.h>
#include <can_service.h>

void print_error(const char *msg)
{
    while (true)
    {
        PRINTF("%s\n", msg);
        bsp_delay(1000);
    }
}

void setup()
{
#ifdef DEVELOPMENT
    bsp_serial_begin();
    bsp_delay(4000);
#endif

    scheduler_init(NULL, NULL);

    if (!scheduler_add_task("Watering", watering_init, watering_run, 0, WATERING_INTERVAL))
    {
        print_error("Failed to add Watering application to the scheduler");
    }

    if (!scheduler_add_task("Ambient", ambient_init, ambient_run, 1, AMBIENT_INTERVAL))
    {
        print_error("Failed to add Ambient application to the scheduler");
    }

    if (!scheduler_add_task("CAN Service", can_service_init, can_service_run, 2, CAN_INTERVAL))
    {
        print_error("Failed to add CAN service to the scheduler");
    }

    if (!scheduler_run())
    {
        print_error("Failed to start the scheduler");
    }
}

void loop()
{
}