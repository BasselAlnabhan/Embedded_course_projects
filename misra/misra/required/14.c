/**
 * The right hand operand of a shift operator shall lie in the range zero to one less than 
 * the width in bits of the essential type of the left hand operand
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
    uint8_t u8a = 1;
    uint16_t u16a = 1;

    u8a = u8a << 7; /* Compliant */
    u8a = u8a << 8; /* Non-compliant */

    u16a = (uint16_t)u8a << 9; /* Compliant */

    1u << 10u;           /* Non-compliant */
    (uint16_t)1u << 10u; /* Compliant */
    1UL << 10u;          /* Compliant */

    return 0;
}