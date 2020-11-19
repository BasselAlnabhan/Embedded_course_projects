#include <RNGA.h>
#include <kinetis.h>

#define RNGA_CR_GO ((uint32_t)0x00000001)
#define RNGA_CR_HA ((uint32_t)0x00000002)
#define RNGA_CR_INTM ((uint32_t)0x00000004)
#define RNGA_CR_SLP ((uint32_t)0x00000010)
#define RNGA_SR_OREG_LVL ((uint32_t)0x0000FF00)

static uint32_t seed_value = 0;

void rnga_init(void)
{
    SIM_SCGC6 |= SIM_SCGC6_RNGA; // enable RNGA clock
    RNG_CR |= RNGA_CR_INTM;
    RNG_CR |= RNGA_CR_HA;
    RNG_CR |= RNGA_CR_GO;
}

void rnga_seed(uint32_t seed)
{
    seed_value = seed;
}

uint32_t rnga_generate(uint32_t min, uint32_t max)
{
    RNG_CR &= ~RNGA_CR_SLP; // change to normal mode
    RNG_ER = seed_value;

    while (!(RNG_SR & RNGA_SR_OREG_LVL))
    {
    }
    RNG_CR |= RNGA_CR_SLP; // change to sleep mode to save the power
    return (min + RNG_OR % (max - min));
}