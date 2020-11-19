/**
 * Controlling expressions shall not be invariant
 *  Exceptions:
 *      1. Invariants that are used to create infi nite loops are permitted.
 *      2. A do ... while loop with an essentially Boolean controlling expression that evaluates to 0 is permitted.
 * 
 * The controlling expression of an if statement and the controlling expression of 
 * an iteration-statement shall have essentially Boolean type
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    uint16_t u16a = 1;
    int8_t s8a = (u16a < 0u) ? 0 : 1; /* Non-compliant - u16a always >= 0 */

    if (u16a <= 0xffffu) /* Non-compliant - always true */
    {
    }

    while (true) /* Compliant by exception 1 */
    {
    }

    do
    {
    } while (0u == 1u); /* Compliant by exception 2 */

    const uint8_t numcyl = 4u;
    if (numcyl == 4u) /* Non-compliant - numcyl always has value 4 */
    {
    }

    const volatile uint8_t numcyl_cal = 4u;
    if (numcyl_cal == 4u)
    {
        /*
        * Compliant - compiler assumes numcyl_cal may be changed by
        * an external method, e.g. automotive calibration tool, even
        * though the program cannot modify its value
        */
    }

    int32_t i;
    int32_t *p, *q;

    while (p) /* Non-compliant - p is a pointer */
    {
    }

    while (q != NULL) /* Compliant */
    {
    }

    while (true) /* Compliant */
    {
    }

    if (i) /* Non-compliant - i is an int32_t */
    {
    }

    return 0;
}