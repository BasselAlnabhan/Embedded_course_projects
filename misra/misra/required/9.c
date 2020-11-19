/**
 * The initializer for an aggregate or union shall be enclosed in braces
 *  Exceptions: 
 *      1. An array may be initialized using a string literal.
 *      2. A designated initializer may be used to initialize part of a subobject
 *      3. An automatic structure or union may be initialized using an expression with compatible structure or union type.
 * 
 * Arrays shall not be partially initialized
 *  Exceptions:
 *      1. An initializer of the form { 0 } may be used to explicitly initialize all elements of an array object or subobject.
 *      2. An array whose initializer consists only of designated initializers may be used
 *      3. An array initialized using a string literal does not need an initializer for every element.
 * 
 * An element of an object shall not be initialized more than once
 * 
 * Where designated initializers are used to initialize an array, the size of the array shall be specified explicitly
 */

#include <stdio.h>
#include <stdint.h>

#define PI (3.1415f)

uint16_t *p;

struct mystruct
{
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
};

void func(void)
{
    uint16_t a[2] = {[0] = *p++, [0] = 1}; /* Non-compliant - it is unspecified whether the side effect occurs or not */
}

int main()
{
    int16_t a[3][2] = {1, 2, 0, 0, 5, 6};       /* Non-compliant */
    int16_t b[3][2] = {{1, 2}, {0}, {5, 6}};    /* Compliant */
    int16_t c[3][2] = {{1, 2}, {0, 0}, {5, 6}}; /* Compliant */

    int16_t d[2][2] = {{0}, [1][1] = 1};             /* Compliant */
    int16_t e[2][2] = {{0}, [1][1] = 1, [1][0] = 0}; /* Compliant */
    int16_t f[2][2] = {{0}, [1][0] = 0, 1};          /* Non-compliant */
    int16_t g[2][2] = {[0][1] = 1, {0, 1}};          /* Compliant */

    float h[3][2] = {0};             /* Compliant */
    float i[3][2] = {{0}, {0}, {0}}; /* Compliant */
    float j[3][2] = {{0.0f, 0.0f},
                     {0.0f, 0.0f},
                     {0.0f, 0.0f}}; /* Compliant */

    union u1 {
        int16_t i;
        float f;
    } u = {0}; /* Compliant */

    struct s1
    {
        uint16_t len;
        char buf[8];
    } s[3] = {
        {5u, {'a', 'b', 'c', 'd', 'e', '\0', '\0', '\0'}},
        {2u, {0}},
        {.len = 0u} /* Compliant - buf initialized implicitly */
    };              /* Compliant - s[] fully initialized */

    int32_t x[3] = {0, 1, 2}; /* Compliant */

    int32_t y[3] = {0, 1}; /* Non-compliant - y[2] is implicitly initialized */

    float t[4] = {[1] = 1.0f, 2.0f}; /* Non-compliant - t[ 0 ] and t[ 3 ] are implicitly initialized */

    float z[50] = {[1] = 1.0f, [25] = 2.0f}; /* Compliant - designated initializers for sparse matrix */

    float arr[3][2] = {
        {0.0f, 0.0f},
        {PI / 4.0f, -PI / 4.0f},
        {0} /* initializes all elements of array subobject arr[ 2 ] */
    };

    char p[10] = "Hello"; /* Compliant by Exception 3 */

    int16_t a1[5] = {-5, -4, -3, -2, -1};                               /* Compliant */
    int16_t a2[5] = {[0] = -5, [1] = -4, [2] = -3, [2] = -2, [4] = -1}; /* Non-compliant - a2[2] has been initialized twice */

    struct mystruct s1 = {100, -1, 42, 999};                     /* Compliant */
    struct mystruct s2 = {.a = 100, .b = -1, .c = 42, .d = 999}; /* Compliant */
    struct mystruct s3 = {.a = 100, .b = -1, .a = 42, .d = 999}; /* Non-compliant - a has been initialized twice */

    int a3[] = {[0] = 1};   /* Non-compliant - probably unintentional to have single element */
    int a4[10] = {[0] = 1}; /* Compliant */

    return 0;
}