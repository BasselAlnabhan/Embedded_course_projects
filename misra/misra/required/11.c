/**
 * Conversions shall not be performed between a pointer to a function and any other type
 * 
 *  Exceptions:
 *      1. A null pointer constant may be converted into a pointer to a function;
 *      2. A pointer to a function may be converted into void;
 *      3. A function type may be implicitly converted into a pointer to that function type.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef void (*fp16)(int16_t n);
typedef void (*fp32)(int32_t n);
typedef fp16 (*pfp16)(void);
extern void f(int16_t n);

int main()
{
    fp16 fp1 = NULL;      /* Compliant - exception 1 */
    fp32 fp2 = (fp32)fp1; /* Non-compliant - function pointer into different function pointer */

    if (fp2 != NULL) /* Compliant - exception 1 */
    {
        fp16 fp3 = (fp16)0x8000; /* Non-compliant - integer into function pointer */
        fp16 fp4 = (fp16)1.0e6F; /* Non-compliant - float into function pointer */
    }

    pfp16 pfp1;

    (void)(*pfp1()); /* Compliant - exception 2, cast function pointer into void */
    fp16 fp5 = f;    /* Compliant - exception 3 - implicit conversion of f into pointer to function */

    return 0;
}