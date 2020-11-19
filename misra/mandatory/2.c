/**
 * The operand of the sizeof shall not contain any expression which has potential side effects
 * 
 * Exception: An expression of the form sizeof(V), where V is an lvalue with a volatile 
 *            qualified type that is not a variable-length array, is permitted
 * 
 * NB: volatile objects have side effects
 */

#include <stdio.h>
#include <stdint.h>

volatile uint32_t v;

void func(int32_t n)
{
    size_t s;
    s = sizeof(int32_t[n]);                /* Compliant */
    s = sizeof(int32_t[n++]);              /* Non-compliant */
    s = sizeof(void (*[n])(int32_t a[v])); /* Non-compliant - the length of the array is variable */
}

int main()
{
    size_t s;
    int32_t j = 10;

    s = sizeof(j);        /* Compliant */
    s = sizeof(j++);      /* Non-compliant */
    s = sizeof(v);        /* Compliant - exception */
    s = sizeof(uint32_t); /* Compliant */

    return 0;
}