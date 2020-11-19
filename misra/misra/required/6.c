/**
 * A tag name shall be a unique identifier
 *      Exception: The tag name may be the same as the typedef name with which it is associated
 */

#include <stdio.h>
#include <stdint.h>

struct deer
{
    uint16_t a;
    uint16_t b;
};

void func(void)
{
    struct deer /* Non-compliant - tag "deer" reused */
    {
        uint16_t a;
    };
}

typedef struct coord
{
    uint16_t x;
    uint16_t y;
} coord; /* Compliant - by exception */

int main()
{
    return 0;
}