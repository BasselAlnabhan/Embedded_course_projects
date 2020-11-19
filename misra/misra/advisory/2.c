/**
 * A project should not contain unused tag declarations
 * 
 * When an array with external linkage is declared, its size should be explicitly specified
 * 
 * A pointer should point to a const-qualified type whenever possible
 *      A pointer should point to a const-qualified type unless either:
 *          • It is used to modify an object, or
 *          • It is copied to another pointer that points to a type that is not const-qualified by means of
 *              either assignment or memory move or copying functions.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct record_t /* Non-compliant - record is an unused tag */
{
    uint16_t key;
    uint16_t val;
} record1_t;

typedef struct
{
    uint16_t key;
    uint16_t val;
} record2_t; /* Compliant */

void unusedtag(void)
{
    enum state
    {
        S_init,
        S_run,
        S_sleep
    }; /* Non-compliant - state is an unused tag */
}

extern int32_t array1[10]; /* Compliant */
extern int32_t array2[];   /* Non-compliant */

uint16_t funcA(uint16_t *p) /* Non-compliant - p is not used to modify an object */
{
    return *p;
}

uint16_t funcB(const uint16_t *p) /* Compliant */
{
    return *p;
}

char funcC(char *const s) /* Non-compliant - s is const-qualified but the type it points to is not */
{
    return s[strlen(s) - 1u];
}

char funcD(const char *const s) /* Compliant - s is const pointer to a const char */
{
    return s[strlen(s) - 1u];
}

uint16_t firstE(uint16_t a[5]) /* Non-compliant - none of the elements of the array a are modified */
{
    return a[0];
}

uint16_t firstF(const uint16_t a[5]) /* Compliant */
{
    return a[0];
}

int main()
{
    return 0;
}