/**
 * The right hand operand of a logical && or || operator shall not contain persistent side effects
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint16_t f(uint16_t y)
{
    /* These side effects are not persistent as seen by the caller */
    uint16_t temp = y;
    temp = y + 0x8080U;
    return temp;
}

uint16_t h(uint16_t y)
{
    static uint16_t temp = 0;

    /* This side effect is persistent */
    temp = y + temp;

    return temp;
}

void g(bool ishigh)
{
    uint16_t value = 0;
    /* Compliant - f ( ) has no persistent side effects */
    if (ishigh && (1 == f(value)))
    {
    }
    /* Non-compliant - h ( ) has a persistent side effect */
    if (ishigh && (1 == h(value)))
    {
    }
}

int main()
{
    uint16_t x;
    volatile uint16_t v;

    /* Non-compliant - access to volatile v is persistent */
    if ((x == 0u) || (v == 1u))
    {
    }

    /* Non-compliant if fp points to a function with persistent side effects */
    (fp != NULL) && (*fp)(0);

    return 0;
}