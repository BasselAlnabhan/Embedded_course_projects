/**
 * Function types shall be in prototype form with named parameters
 * 
 * All declarations of an object or function shall use the same names and type qualifiers
 * 
 * An object should be defined at block scope if it's identifier only appears in a single function
 * 
 * An inline function shall be declared with the static storage class
 * 
 * A pointer should point to a const-qualified type whenever possible
 */

#include <stdio.h>
#include <stdint.h>

extern int16_t func1(int16_t n); /* Compliant */

/* Non-compliant - parameter name not specified */
extern void func2(int16_t);

/* Non-compliant - not in prototype form */
static int16_t func3();

/* Compliant - prototype specifies 0 parameters */
static int16_t func4(void);

extern int16_t func(int16_t num, int16_t den);

/* Non-compliant - parameter names do not match */
int16_t func(int16_t den, int16_t num)
{
    return num / den;
}

void func(void)
{
    int32_t i; /* Complaint - "i" is the scope of the function */
    for (i = 0; i < 10; ++i)
    {
    }
}

/* Complaint - static inline function */
static inline uint16_t square(uint8_t n)
{
    return ((uint16_t)n * n);
}

/* Non-compliant - p is not used to modify an object */
uint16_t f(uint16_t *p)
{
    return *p;
}

/* Non-compliant - s should be a const pointer to a const char */
char last_char(char *const s)
{
    return s[strlen(s) - 1u];
}

/* Non-compliant - a should be a constant */
uint16_t first(uint16_t a[5])
{
    return a[0];
}

int main()
{
    return 0;
}