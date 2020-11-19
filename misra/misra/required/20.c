/**
 * All switch statements shall be well-formed
 *      switch ( switch-expression ) { case-label-clause-list final-default-clause-list }
 *      switch ( switch-expression ) { initial-default-clause-list case-label-clause-list }
 * 
 * An unconditional break statement shall terminate every switch-clause
 * 
 * Every switch statement shall have a default label
 * 
 * A default label should appear as either the first or the last switch label of a switch statement
 * 
 * Every switch statement shall have at least two switch-clauses
 * 
 * A switch-expression shall not have essentially Boolean type
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    uint8_t x = 5;

    switch (x)
    {
    case 0:
        break; /* Compliant - unconditional break */
    case 1:    /* Compliant - empty fall through allows a group */
    case 2:
        break; /* Compliant - unconditional break */
    case 4:
        a = b; /* Non-compliant - break omitted */
    case 5:
        if (a == b)
        {
            ++a;
            break; /* Non-compliant - conditional break */
        }
    default:; /* Non-compliant - default must also have a break */
    }

    switch (x) /* Non-compliant - at least two switch-clauses are required */
    {
    default:
        x = 0;
        break;
    }

    switch (y) /* Non-compliant - at least two switch-clauses are required */
    {
    case 1:
    default:
        y = 0;
        break;
    }

    switch (z) /* Compliant */
    {
    case 1:
        z = 2;
        break;
    default:
        z = 0;
        break;
    }

    switch (x == 0) /* Non-compliant - essentially Boolean */
    {               /* In this case an "if-else" would be more logical */
    case false:
        y = x;
        break;
    default:
        y = z;
        break;
    }

    return 0;
}