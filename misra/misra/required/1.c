/**
 * Run-time failures shall be minimized
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
    float f1 = 1E38f;
    float f2 = 10.0f;
    float f3 = 0.1f;
    float f4 = (f1 * f2) * f3; /* (f1 * f2) will overflow */
    float f5 = f1 * (f2 * f3); /* No overflow because (f2 * f3) is (approximately) 1 */

    if ((f3 >= 0.0f) && (f3 <= 1.0f))
    {
        /*
        * No overflow because f3 is known to be in range 0..1 so the
        * result of the multiplication will fit in type float
        */
        f4 = f3 * 100.0f;
    }

    uint8_t *ptr = malloc(sizeof(uint8_t));
    *ptr = 0U; /* Non-compliant - it is essential to check that each allocation succeeds */

    return 0;
}