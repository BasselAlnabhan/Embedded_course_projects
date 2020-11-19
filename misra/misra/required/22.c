/**
 * The address of an object with automatic storage shall not be copied to 
 * another object that persists after the first object has ceased to exist
 * 
 * Flexible array members shall not be declared
 * 
 * Variable-length array types shall not be used
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int8_t *func(void)
{
    int8_t local_auto = 1;

    return &local_auto; /* Non-compliant - &local_ auto is indeterminate when func returns */
}

uint16_t *sp;

void g(uint16_t *p)
{
    sp = p; /* Non-compliant - address of g's parameter copied to static sp */
}

struct s
{
    uint16_t len;
    uint32_t data[]; /* Non-compliant - flexible array member */
} str;

void f(int16_t n)
{
    uint16_t vla[n]; /* Non-compliant - Undefined if n <= 0 */
}

void h(uint16_t n, uint16_t a[10][n]) /* Non-compliant */
{
    uint16_t(*p)[20];

    p = a; /* Undefined if n != 20 */
}

int main()
{
    return 0;
}