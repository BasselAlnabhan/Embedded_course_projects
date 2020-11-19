/**
 * There should be no more than one break statement used to terminate any iteration statement
 * 
 * A function should have a single point of exit at the end
 * 
 * The function argument corresponding to a parameter declared to have an array type shall have an appropriate number of elements
 * 
 * A function parameter should not be modified
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Non-Compliant */
bool f(uint16_t n, char *p)
{
    if (n > MAX)
    {
        return false;
    }
    if (p == NULL)
    {
        return false;
    }
    return true;
}

void fn1(int32_t array1[4]); /* Intent is that function handles arrays of 4 elements */
void fn2(int32_t array2[]);  /* Intent is that function handles arrays of any size */

void fn(int32_t *ptr)
{
    int32_t arr3[3] = {1, 2, 3};
    int32_t arr4[4] = {0, 1, 2, 3};

    fn1(arr4); /* Compliant - size of array matches the prototype */
    fn1(arr3); /* Non-compliant - size of array does not match prototype */
    fn1(ptr);  /* Compliant - only if ptr points to at least 4 elements */
    fn2(arr4); /* Compliant */
    fn2(ptr);  /* Compliant */
}

int16_t glob = 0;
void proc(int16_t para)
{
    para = glob; /* Non-compliant */
}
void f(char *p, char *q)
{
    p = q;   /* Non-compliant */
    *p = *q; /* Compliant */
}

int main()
{
    /* Compliant - every loop has a single break */
    for (x = 0; x < LIMIT; ++x)
    {
        if (ExitNow(x))
        {
            break;
        }
        for (y = 0; y < x; ++y)
        {
            if (ExitNow(LIMIT - y))
            {
                break;
            }
        }
    }

    /* Non-Compliant */
    for (x = 0; x < LIMIT; ++x)
    {
        if (BreakNow(x))
        {
            break;
        }
        else if (GotoNow(x))
        {
            break;
        }
        else
        {
            KeepGoing(x);
        }
    }

    return 0;
}