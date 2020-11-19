/**
 * An object with auto storage class shall not be read before it has been set
 *      Objects with static storage duration are automatically initialized to zero unless initialized explicitly
 *      Objects with automatic storage are not automatically initialized and can therefore have indeterminate values
 *      Explicit initialization of an automatic object can be ignored (the example)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void func(bool b, uint16_t *ptr)
{
    if (b)
    {
        *ptr = 3U;
    }
}

int main()
{
    uint16_t x;
    uint16_t value;

    func(false, &value);
    if (value == 3U)
    {
        /* Non-compliant - value has not been initialized */
    }

    printf("%d\n", x); /* Non-compliant - x has not been initialized */

    return 0;
}