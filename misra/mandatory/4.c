/**
 * The declaration of an array parameter shall not contain the static keyword between the [ ]
 * 
 * The C99 language standard provides a mechanism for the programmer to inform the compiler that 
 * an array parameter contains a specified minimum number of elements. Some compilers are able 
 * to take advantage of this information to generate more efficient code for some types of processor.
 * 
 * If the guarantee made by the programmer is not honoured, and the number of elements 
 * is less than the minimum specified, the behaviour is undefined.
 */

#include <stdio.h>
#include <stdint.h>

extern uint16_t v1[10];
extern uint16_t v2[20];

/* Non-compliant - uses static in array declarator */
uint16_t total(uint16_t n, uint16_t a[static 20])
{
    uint16_t sum = 0U;

    /* Undefined behaviour if a has fewer than 20 elements */
    for (uint16_t i = 0U; i < n; ++i)
    {
        sum = sum + a[i];
    }

    return sum;
}

void g(void)
{
    uint16_t x;
    x = total(10U, v1); /* Undefined - v1 has 10 elements but needs at least 20 */
    x = total(20U, v2); /* Defined but non-compliant */
}

int main()
{
    return 0;
}