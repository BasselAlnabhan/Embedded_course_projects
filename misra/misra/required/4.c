/**
 * Line-splicing shall not be used in // comments
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern bool b;

void f(void)
{
    uint16_t x = 0; // comment \
    if (b)
    {
        ++x; /* This is always executed */
    }
}

int main()
{
    return 0;
}