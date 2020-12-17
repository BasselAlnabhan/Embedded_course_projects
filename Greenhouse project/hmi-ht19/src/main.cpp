#include <bsp.h>
#include <common.h>
#include <bsp_time.h>
#include <scheduler.h>
#include <can_service.h>
#include <system_status.h>
#include <menu_system.h>

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
    bsp_delay(1000);
#endif

    scheduler_init(menu_system_init, menu_system_run);

    if (!scheduler_add_task("CAN Service", can_service_init, can_service_run, 2, CAN_INTERVAL))
    {
        print_error("Failed to add CAN service to the scheduler");
    }
    if (!scheduler_add_task("System Status", system_status_init, system_status_run, 3, 100U))
    {
        print_error("Failed to add System Status to the scheduler");
    }
    if (!scheduler_run())
    {
        print_error("Failed to start the scheduler");
    }
}

void loop()
{
}
