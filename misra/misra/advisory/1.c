/**
 * Sections of code should not be “commented out”
 * 
 * typedefs that indicate size and signedness should be used in place of the basic numerical types
 *  Exceptions: 
 *      1. The basic numerical types may be used in a typedef to define a specific-length type
 *      2. int and char is permitted in int main( int argc, char *argv[])
 * 
 * A function should be used in preference to a function-like macro where they are interchangeable
 *  NB: A function-like macro is evaluated in compile time but a function is called in the run time
 */

#include <stdio.h>
#include <stdint.h>

#define DIV2(X) ((X) / 2)
#define EVAL_BINOP(OP, L, R) ((L)OP(R))

typedef int SINT_16;      /* Compliant - int used to define specific-length type */
typedef int speed_t;      /* Non-compliant - no sign or size specified */
typedef int16_t torque_t; /* Compliant - further abstraction does not need specific length */

uint32_t y = EVAL_BINOP(+, 1, 2); /* Compliant - The function-like macro cannot be replaced with a function */

static inline div_by_2(uint16_t num)
{
    return (num / 2);
}

void f(void)
{
    static uint16_t x = DIV2(10); /* Compliant - function call not permitted */
    uint16_t y = DIV2(10);        /* Non-compliant - function call permitted */
    uint16_t z = div_by_2(10);    /* Compliant - function call is used */
}

int main()
{
    int x = 0; /* Non-compliant - int used to define an object */
    uint8_t i = 0;

    /* Non-compliant */
    // i++;
    // printf("%d\n", i);

#if 0
    i++;
    printf("%d\n", i);
#endif

    return 0;
}