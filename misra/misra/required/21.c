/**
 * A pointer resulting from arithmetic on a pointer operand shall address an element of the same array as that pointer operand
 * 
 * Subtraction between pointers shall only be applied to pointers that address elements of the same array
 * 
 * The relational operators >, >=, < and <= shall not be applied to objects of pointer type except where they point into the same object
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int32_t f1(int32_t *const a1, int32_t a2[10])
{
    int32_t *p = &a1[3]; /* Compliant/non-compliant depending on the number of the elements of a1 */
    return *(a2 + 9);    /* Compliant */
}

void f2(void)
{
    int32_t array[10] = {0};

    int32_t *p1 = &array[0];  /* Compliant */
    int32_t *p2 = &array[10]; /* Non-compliant - undefined, points to one beyond */
    int32_t data = *p2;       /* Non-compliant - undefined, dereference one beyond */
    p1++;                     /* Compliant */
    array[-1] = 0;            /* Non-compliant - undefined, array bounds exceeded */
}

void f3(int32_t *ptr)
{
    int32_t a1[10];
    int32_t a2[10];

    int32_t *p1 = &a1[1];
    int32_t *p2 = &a2[10];

    ptrdiff_t diff;

    diff = p1 - a1;  /* Compliant */
    diff = p2 - a2;  /* Compliant */
    diff = p1 - p2;  /* Non-compliant */
    diff = ptr - p1; /* Non-compliant */
}

void f3(void)
{
    int32_t a1[10];
    int32_t a2[10];

    int32_t *p1 = a1;

    if (p1 < a1) /* Compliant */
    {
    }
    if (p1 < a2) /* Non-compliant */
    {
    }
}

struct limits
{
    int32_t lwb;
    int32_t upb;
};

void f2(void)
{
    struct limits limits_1 = {2, 5};
    struct limits limits_2 = {10, 5};

    if (&limits_1.lwb <= &limits_1.upb) /* Compliant */
    {
    }

    if (&limits_1.lwb > &limits_2.upb) /* Non-Compliant */
    {
    }
}

int main()
{
    return 0;
}