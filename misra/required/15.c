/**
 * Initializer lists shall not contain persistent side effects
 * 
 * The value of an expression and its persistent side effects shall be the same under all permitted evaluation orders
 * Between any two adjacent sequence points or within any full expression:
 *      1. No object shall be modifi ed more than once;
 *      2. No object shall be both modified and read unless any such read of the object’s value 
 *         contributes towards computing the value to be stored into the object;
 *      3. There shall be no more than one modification access with volatile-qualified type;
 *      4. There shall be no more than one read access with volatile-qualified type.
 */

#include <stdio.h>
#include <stdint.h>

#define COPY_ELEMENT(a, b, index) (a[(index)] = b[(index)])

uint16_t x = 0u;
volatile uint16_t v1;
extern void p(uint16_t a[2]);
extern volatile uint16_t v1, v2;
extern volatile uint8_t PORT;

void f(void)
{
    uint16_t a[2] = {v1, 0}; /* Non-compliant - volatile access is persistent side effect */
}
void g(uint16_t x, uint16_t y)
{
    uint16_t a[2] = {x + y, x - y}; /* Compliant - no side effects */
}
void h(void)
{
    p((uint16_t[2]){x++, x++}); /* Non-compliant - two side effects */
}

uint8_t func(uint8_t x, uint8_t y)
{
    return (x + y);
}

int main()
{
    uint8_t i = 0;
    uint8_t arra[4] = {0, 0, 0, 0};
    uint8_t arrb[4] = {0, 0, 0, 0};

    COPY_ELEMENT(arra, arrb, i++); /* Non-compliant - i is read twice and modifi ed twice */
    uint16_t t = v1 + v2;          /* Non-compliant - the order in which v1 and v2 are read is unspecified */

    PORT = PORT & 0x80u; /* Compliant - no side effects, PORT is read and modified */

    func(i++, i); /* Non-compliant - the order of evaluation of function arguments is unspecifi ed */

    /* The relative order of evaluation of a function designator and function arguments is unspecifi ed. */
    p->f(g(&p)); /* Non-compliant - if g modifies p then it is unspecified whether p->f uses the value of p prior to the call of g or after it. */

    return 0;
}