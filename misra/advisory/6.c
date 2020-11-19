/**
 * The +, -, += and -= operators should not be applied to an expression of pointer type
 * 
 * Declarations should contain no more than two levels of pointer nesting
 */

#include <stdio.h>
#include <stdint.h>

typedef int8_t *INTPTR;

struct s
{
    int8_t *s1;   /* Compliant */
    int8_t **s2;  /* Compliant */
    int8_t ***s3; /* Non-compliant */
};

void func(int8_t **arrPar[]) /* Non-compliant */
{
    int8_t **obj2;             /* Compliant */
    int8_t ***obj3;            /* Non-compliant */
    INTPTR *obj4;              /* Compliant */
    INTPTR *const *const obj5; /* Non-compliant */
}

struct s *ps1;   /* Compliant */
struct s **ps2;  /* Compliant */
struct s ***ps3; /* Non-compliant */

int8_t **(*pfunc1)(void);   /* Compliant */
int8_t **(**pfunc2)(void);  /* Compliant */
int8_t **(***pfunc3)(void); /* Non-compliant */
int8_t ***(**pfunc4)(void); /* Non-compliant */

int main()
{
    uint8_t a[10];
    uint8_t *ptr;
    uint8_t index = 0U;
    index = index + 1U; /* Compliant - rule only applies to pointers */
    a[index] = 0U;      /* Compliant */
    ptr = &a[5];        /* Compliant */

    ptr = a;
    ptr++;           /* Compliant - increment operator not + */
    *(ptr + 5) = 0U; /* Non-compliant */
    ptr[5] = 0U;     /* Compliant */

    return 0;
}