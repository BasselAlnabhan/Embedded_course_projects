/**
 * A project shall not contain unreachable code
 * 
 * A project shall not contain dead code
 *      Exception: A cast to void is used to indicate a value is intentionally not used.
 */

#include <stdio.h>
#include <stdint.h>

extern volatile uint16_t u16a;

uint8_t funcA(uint8_t a, uint8_t b)
{
    (void)a; /* Compliant - by the exception */

    return b;
}

void funcB(void)
{
    /* Compliant - there are no operations in this function */
}

void funcC(void)
{
    funcB(); /* Non-compliant - the call could be removed */
}

int main()
{
    uint8_t result = 0;

    (uint32_t) u16a; /* Non-compliant - dead code */
    u16a >> 3;       /* Non-compliant - dead code */

    return result;

    result = 1; /* Non-compliant - this statement is unreachable */
}