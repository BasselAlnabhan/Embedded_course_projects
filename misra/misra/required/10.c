/**
 * The value of an expression shall not be assigned to an object with a 
 * narrower essential type or of a different essential type category
 *  Exceptions:
 *      1. A non-negative integer constant of signed type may be assigned to an unsigned type
 *      2. The initializer { 0 } may be used to initialize an aggregate or union type
 * 
 * Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category
 *      Exception: + , -, += and -= in characters with signed or unsigned type
 * 
 * The value of a composite expression shall not be assigned to an object with wider essential type
 * 
 * The value of a composite expression shall not be cast to a different essential type category or a wider essential type
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
    char chr = 'A';
    uint8_t a = 1; /* Compliant - by exception */

    chr = chr + 1; /* Compliant - assigns character to character */

    enum enuma
    {
        K1 = 1,
        K2 = 128
    }; /* Compliant - by exception */

    uint16_t u16a = 2U;
    uint16_t u16b = 2U;

    uint32_t u32a = 2U;
    uint32_t u32b = 2U;

    int32_t s32a = 1;
    int32_t s32b = -1;

    uint8_t b = 1.0f; /* Non-compliant - unsigned and floating */
    u16a = u32a;      /* Non-compliant - uint32_t to uint16_t */

    uint16_t u16c = u16a + u16b;    /* Compliant - same essential type */
    u32a = (uint32_t)u16a + u16b;   /* Compliant - cast causes the addition in uint32_t */
    u32a = u16a + u16b;             /* Non-compliant - implicit conversion on the assignment */
    u16a = (uint16_t)(u32a + u32b); /* Compliant */
    u16a = (uint16_t)(s32a + s32b); /* Non-compliant - different type category */
    u32a = (uint32_t)(u16a + u16b); /* Non-compliant - cast to a wider essential type */

    return 0;
}