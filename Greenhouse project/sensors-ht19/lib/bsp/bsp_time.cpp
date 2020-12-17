#include <bsp.h>
#include <bsp_time.h>
#include <IntervalTimer.h>

#define MICROSEC (1000U)
#define TIMER_PRIORITY (5U)

static IntervalTimer timer;
static void (*callback)(void);

void bsp_delay(uint32_t msec)
{
    delay(msec);
}

void bsp_timer_stop(void)
{
    timer.end();
}

bool bsp_timer_start(void)
{
    bool status = false;
    if (callback)
    {
        timer.priority(TIMER_PRIORITY);
        status = timer.begin(callback, MICROSEC);
    }
    return status;
}

void bsp_timer_init(void (*func)(void))
{
    callback = func;
}