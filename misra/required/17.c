/**
 * A loop counter shall not have essentially floating type
 * 
 * A for loop shall be well-formed
 * The three clauses of a for statement are the:
 *      First clause which
 *          • Shall be empty, or
 *          • Shall assign a value to the loop counter, or
 *          • Shall define and initialize the loop counter (C99).
 *      Second clause which
 *          • Shall be an expression that has no persistent side effects, and
 *          • Shall use the loop counter and optionally loop control flags, and
 *          • Shall not use any other object that is modified in the for loop body.
 *      Third clause which
 *          • Shall be an expression whose only persistent side effect is to modify the value of the loop counter, and
 *          • Shall not use objects that are modified in the for loop body.
 *      There shall only be one loop counter in a for loop, which shall not be modified in the for loop body.
 *      
 *      Exception: All three clauses may be empty, i.e. for(;;), to allow for infinite loops.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    uint32_t counter = 0u;
    for (float f = 0.0f; f < 1.0f; f += 0.001f) /* Non-compliant - the value of counter is unlikely to be 1000 at the end of the loop */
    {
        ++counter;
    }

    float f;
    for (uint32_t i = 0u; i < 1000u; ++i) /* Compliant */
    {
        f = (float)i * 0.001f;
    }

    f = 0.0f;
    while (f < 1.0f) /* Non-compliant - f is being used as a loop counter. */
    {
        f += 0.001f;
    }

    uint32_t u32a;
    f = read_float32();
    do
    {
        u32a = read_u32();

    } while (((float)u32a - f) > 10.0f); /* Compliant - f does not change in the loop so cannot be a loop counter */

    uint8_t C = 1;
    bool flag = false;
    for (int16_t i = 0; (i < 5) && !flag; i++)
    {
        if (C)
        {
            flag = true; /* Compliant - allows early termination of loop */
        }
        i = i + 3; /* Non-compliant - altering the loop counter */
    }

    return 0;
}