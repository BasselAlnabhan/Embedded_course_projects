#include <RNGA.h>
#include <kinetis.h>

#define RNGA_CR_GO ((uint32_t)0x00000001)
#define RNGA_CR_HA ((uint32_t)0x00000002)
#define RNGA_CR_INTM ((uint32_t)0x00000004)
#define RNGA_CR_SLP ((uint32_t)0x00000010)
#define RNGA_SR_OREG_LVL ((uint32_t)0x0000FF00)

void rnga_init(void)
{
    SIM_SCGC6 |= SIM_SCGC6_RNGA; // enable RNGA clock
}

void rnga_seed(uint32_t seed)
{
}

uint32_t rnga_generate(uint32_t min, uint32_t max)
{
}