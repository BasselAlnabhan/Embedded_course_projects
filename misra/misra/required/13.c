/**
 * A cast shall not be performed between pointer to void and an arithmetic type
 *  Exception: An integer constant expression with value 0 may be cast into pointer to void.
 * 
 * A cast shall not be performed between pointer to object and a non-integer arithmetic type
 * 
 * A cast shall not remove any const or volatile qualification from the type pointed to by a pointer
 * 
 * The macro NULL shall be the only permitted form of integer null pointer constant
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern void func(uint8_t *p);

int main()
{
    void *p;
    uint32_t u;

    p = (void *)0x1234u; /* Non-compliant - implementation-defined */
    p = (void *)1024.0f; /* Non-compliant - undefined */
    u = (uint32_t)p;     /* Non-compliant - implementation-defined */
    p = (void *)0;       /* Compliant - NULL */

    float f;
    int16_t *ptr;

    f = (float)p;     /* Non-compliant */
    p = (int16_t *)f; /* Non-compliant */

    uint16_t x;
    uint16_t *const cpi = &x; /* const pointer */
    uint16_t *const *pcpi;    /* pointer to const pointer */
    uint16_t **ppi;
    const uint16_t *pci;    /* pointer to const */
    volatile uint16_t *pvi; /* pointer to volatile */
    uint16_t *pi;

    pi = cpi;                /* Compliant - no conversion no cast required */
    pi = (uint16_t *)pci;    /* Non-compliant */
    pi = (uint16_t *)pvi;    /* Non-compliant */
    ppi = (uint16_t **)pcpi; /* Non-compliant */

    int32_t *p1 = 0;         /* Non-compliant */
    int32_t *p2 = (void *)0; /* Compliant */
    func(NULL);              /* Compliant */

    return 0;
}