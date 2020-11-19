/**
 * An identifier declared in an inner scope shall not hide an identifier declared in an outer scope
 * 
 * Identifiers shall be distinct from macro names
 * 
 * A typedef name shall be a unique identifier
 *      Exception: The typedef name may be the same as the structure, union or enumeration tag name associated with the typedef.
 */

#include <stdio.h>
#include <stdint.h>

#define Sum(x, y) ((x) + (y))

void funcA(void)
{
    int16_t i; /* Declare an object "i" */
    {
        int16_t i; /* Non-compliant - hides previous "i " */
        i = 3;     /* Could be confusing as to which "i" this refers */
    }
}

struct astruct
{
    int16_t m;
};

extern void funcB(struct astruct *p);

int16_t xyz = 0; /* Declare an object "xyz" */

void funcC(struct astruct xyz) /* Non-compliant - outer "xyz" is now hidden by parameter name */
{
    funcB(&xyz);
}

uint16_t speed;

void funcD(void)
{
    typedef float speed; /* Non-compliant - type hides object */
}

void funcE(void)
{
    {
        typedef unsigned char u8_t;
    }
    {
        typedef unsigned char u8_t; /* Non-compliant - reuse */
    }
}

typedef float mass;

void funcF(void)
{
    float mass = 0.0f; /* Non-compliant - reuse */
}

typedef struct list
{
    struct list *next;
    uint16_t element;
} list; /* Compliant - exception */

typedef struct
{
    struct chain
    {
        struct chain *list;
        uint16_t element;
    } s1;
    uint16_t length;
} chain; /* Non-compliant - tag "chain" not associated with typedef */

int main()
{
    uint32_t Sum = 0; /* Non-compliant */

    return 0;
}