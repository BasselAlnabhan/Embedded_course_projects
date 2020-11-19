/**
 * A conversion should not be performed from pointer to void into pointer to object
 *      Exception: A null pointer constant that has type pointer to void may be converted into pointer to object.
 * 
 * The precedence of operators within expressions should be made explicit by parentheses
 * 
 * The comma operator should not be used
 * 
 * Evaluation of constant expressions should not lead to unsigned integer wrap-around
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * On a machine with a 16-bit int type, any value  greater than or equal to 65024 would result in wrap-around
 */
#define DELAY 10000U
#define WIDTH 60000U
void fixed_pulse(void)
{
    uint16_t off_time16 = DELAY + WIDTH; /* Non-compliant - wrap-around, DELAY + WIDTH > 65024 */
}

bool b;
void g(void)
{
    uint16_t x = (0u == 0u) ? 0u : (0u - 1u); /* Compliant */
    uint16_t y = b ? 0u : (0u - 1u);          /* Non-compliant - y can be true or false */
}

int main()
{
    void *p;
    uint32_t *p32;
    uint16_t *p16;

    p = p32;             /* Compliant - pointer to uint32_t into pointer to void */
    p16 = p;             /* Non-compliant */
    p = (void *)p16;     /* Compliant */
    p32 = (uint32_t *)p; /* Non-compliant */

    *p++;                       /* Compliant - no need to write *( p++ ) */
    a[i]->n;                    /* Compliant - no need to write ( a[ i ] )->n */
    sizeof x + y;               /* Non-compliant - write either sizeof ( x ) + y or sizeof ( x + y ) */
    x = f(a + b, c);            /* Compliant - no need to write f ( ( a + b ), c ) */
    x = a == b ? a : a - b;     /* Non-compliant */
    x = (a == b) ? a : (a - b); /* Compliant */
    x = a << (b + c);           /* Compliant */
    if (a && b && c)            /* Compliant */
    {
    }

    f((1, 2), 3); /* Non-compliant - how many parameters? */

    for (i = 0, p = &a[0]; i < 10; ++i, ++p) /* Non-compliant */
    {
    }

    return 0;
}