/**
 * An object shall not be assigned or copied to an overlapping object
 *      The behaviour is undefined when two objects are created which have 
 *      some overlap in memory and one is assigned or copied to the other.
 * 
 * Exception: 
 *      1. Assignment between two objects that overlap exactly and have compatible types (ignoring their type qualifiers) 
 *      2. Copying between objects that overlap partially or completely using The Standard Library function memmove
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int16_t array[20];

void funcA(void)
{
    union {
        int16_t i;
        int32_t j;
    } a = {0}, b = {1};

    a.j = a.i; /* Non-compliant */
    a = b;     /* Compliant - exception 1 */
}

void funcB(void)
{
    memcpy(&array[5], &array[4], 2u * sizeof(array[0])); /* Non-compliant */
}

void funcC(void)
{
    int16_t *p = &array[0];
    int16_t *q = &array[0];
    *p = *q; /* Compliant - exception 1 */
}

int main()
{
    return 0;
}