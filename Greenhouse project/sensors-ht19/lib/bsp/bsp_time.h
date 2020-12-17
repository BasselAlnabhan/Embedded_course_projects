#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <stdint.h>
#include <stdbool.h>

void bsp_delay(uint32_t ms);

void bsp_timer_stop(void);
bool bsp_timer_start(void);
void bsp_timer_init(void (*func)(void));

#endif /* BSP_TIME_H */