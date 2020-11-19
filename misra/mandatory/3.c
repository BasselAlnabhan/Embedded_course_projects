/**
 * All exit paths from a function with non-void return type shall have an explicit return statement with an expression
 */

#include <stdio.h>
#include <stdint.h>

int32_t absolute(int32_t v)
{
    if (v < 0)
    {
        return v;
    }

    /* Non-compliant - control can reach this point without returning a value */
}

uint16_t lookup(uint16_t v)
{
    if ((v < 10) || (v > 1))
    {
        /* Non-compliant - no value returned */
        return;
    }
    return v;
}

int main()
{
    return 0;
}