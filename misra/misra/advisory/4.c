/**
 * A full expression containing an increment (++) or decrement (--) operator should have 
 * no other potential side effects other than that caused by the increment or decrement operator
 * 
 * The result of an assignment operator should not be used
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
    uint8_t u8a = 0;
    uint8_t u8b = 0;
    uint8_t u8c = 0;

    u8a = u8b++;             /* Non-compliant */
    u8a = ++u8b + u8c--;     /* Non-compliant - it can be written: ++u8b; u8a = u8b + u8c; u8c--; */
    if ((f() + --u8a) == 0u) /* Non-compliant */
    {
        g(u8b++); /* Non-compliant */
    }

    if (u8a++ == ((1u == 1u) ? 0u : f())) /* Non-compliant */
    {
        u8a = (1u == 1u) ? 0u : u8b++; /* Non-compliant */
    }

    x++;    /* Compliant - the only side effect is caused by the increment operator */
    a[i]++; /* Compliant - the only side effect is caused by the increment operator */
    b.x++;  /* Compliant - the only side effect is caused by the increment operator */
    c->x++; /* Compliant - the only side effect is caused by the increment operator */
    ++(*p); /* Compliant - the only side effect is caused by the increment operator */
    *p++;   /* Compliant - the only side effect is caused by the increment operator */
    (*p)++; /* Compliant - the only side effect is caused by the increment operator */

    x = y;            /* Compliant */
    a[x] = a[x = y];  /* Non-compliant - the value of x = y is used */
    a[b += c] = a[b]; /* Non-compliant - the value of b += c is used */
    a = b = c = 0;    /* Non-compliant - the values of c = 0 and b = c = 0 are used */

    if (bool_var = false) /* Non-compliant - the value of bool_var = false is used but */
    {
    }

    if ((0u == 0u) || (bool_var = true)) /* Non-compliant - the value of bool_var = true is used but */
    {
    }

    if ((x = f()) != 0) /* Non-compliant - the value of x = f() is used but */
    {
    }

    return 0;
}