/**
 * Bit-fields shall only be declared with an appropriate type (unsigned int, signed int or _Bool)
 * Single-bit named bit fields shall not be of a signed type; one bit can be 0 or 1
 * 
 * A “u” or “U” suffix shall be applied to all integer constants that are represented in an unsigned type
 * 
 * The lowercase character “l” shall not be used in a literal suffix
 * 
 * Octal constants shall not be used
 * 
 * A string literal shall not be assigned to an object unless the object’s type is “pointer to const-qualified char”
 */

#include <stdio.h>
#include <stdint.h>

struct s
{
    unsigned int b1 : 2; /* Compliant */
    int b2 : 2;          /* Non-compliant - plain int not permitted */
    uint16_t b3 : 2;     /* Compliant */
    signed long b4 : 2;  /* Non-compliant - even if long and int are the same size */
};

int main()
{
    const int64_t a = 0L;
    const int64_t b = 0l; /* Non-compliant */
    const uint64_t c = 0Lu;
    const uint64_t d = 0lU; /* Non-compliant */
    const uint64_t e = 0uLL;
    const uint64_t f = 0ll; /* Non-compliant */
    const long double g = 1.2L;
    const long double h = 1.2l; /* Non-compliant */

    int code[10] = {};
    code[0] = 109; /* Compliant - decimal 109 */
    code[1] = 052; /* Non-compliant - decimal 42 */

    /* Compliant - p is const-qualified; additional qualifiers are permitted */
    const volatile char *p = "string";

    /* Non-compliant - s is not const-qualified */
    char *s = "string";

    return 0;
}