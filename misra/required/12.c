/**
 * Conversions shall not be performed between a pointer to an incomplete type and any other type
 * 
 *  Exceptions:
 *      1. A null pointer constant may be converted into a pointer to an incomplete type. 
 *      2. A pointer to an incomplete type may be converted into void.
 * 
 * A cast shall not be performed between a pointer to object type and a pointer to a different object type
 * 
 *  Exception: It is permitted to convert a pointer to object type into 
 *             a pointer to one of the object types char, signed char or unsigned char.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct s; /* Incomplete type */
struct t; /* Another incomplete type */

struct s *f(void);

struct s *sp;
struct t *tp;
int16_t *ip;

extern uint32_t read_value(void);
extern void print(uint32_t n);

void func(void)
{
    uint32_t u = read_value();
    uint16_t *hi_p = (uint16_t *)&u; /* Non-compliant even though probably correctly aligned */
    *hi_p = 0;                       /* Attempt to clear high 16-bits on big-endian machine */
    print(u);                        /* Line above may appear not to have been performed */
}

int main()
{
    ip = (int16_t *)sp;    /* Non-compliant */
    sp = (struct s *)1234; /* Non-compliant */
    tp = (struct t *)sp;   /* Non-compliant - casting pointer into a different incomplete type */

    sp = NULL; /* Compliant - exception 1 */

    (void)f(); /* Compliant - exception 2 */

    uint8_t *p1;
    uint32_t *p2;

    p2 = (uint32_t *)p1; /* Non-compliant - possible incompatible alignment */

    const short *p;
    const volatile short *q;
    q = (const volatile short *)p; /* Compliant */

    int *const *pcpi;
    const int *const *pcpci;
    pcpci = (const int *const *)pcpi; /* Non-compliant - pointer to const-qualified int to pointer to int */

    return 0;
}