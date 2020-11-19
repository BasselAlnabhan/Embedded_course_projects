/**
 * Dynamic memory allocation shall not be used
 * 
 * Sufficient memory may not be available to satisfy a request
 * 
 * There is a high variance in the execution time required to perform allocation or deallocation 
 * depending on the pattern of usage and resulting degree of fragmentation. 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
    char *q;
    char *p = (char *)malloc(10);

    free(p);
    q = p; /* Undefined behaviour - value of p is indeterminate */

    p = (char *)malloc(20);
    free(p);
    p = NULL; /* Assigning NULL to freed pointer makes it determinate */

    return 0;
}