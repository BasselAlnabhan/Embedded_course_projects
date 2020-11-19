/**
 * A block of memory shall only be freed if it was allocated by means of Standard Library function
 * 
 * The Standard Library functions that allocate memory are malloc, calloc and realloc.
 * 
 * Freeing non-allocated memory, or freeing the same allocated memory more than once leads to undefined behaviour.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void funcA(void)
{
    int32_t a;
    /* Non-compliant - a does not point to allocated storage */
    free(&a);
}

void funcB(void)
{
    char *p = (char *)malloc(512);
    char *q = p;

    free(p);

    /* Non-compliant - allocated block freed a second time */
    free(q);

    /* Non-compliant - allocated block may be freed a third time */
    p = (char *)realloc(p, 1024);
}

int main()
{
    return 0;
}