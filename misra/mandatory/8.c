/**
 * A pointer to a FILE object shall not be dereferenced
 * 
 * A pointer to a FILE object shall not be dereferenced directly or indirectly (e.g. by a call to memcpy).
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *pf1;
    FILE *pf2;
    FILE f3;

    pf2 = pf1; /* Compliant */
    f3 = *pf2; /* Non-compliant */

    return 0;
}