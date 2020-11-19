/**
 * The body of an iteration-statement or a selection-statement shall be a compound-statement
 *      Exception: An if statement immediately following an else need not be contained within a compound-statement.
 * 
 * All if ... else if constructs shall be terminated with an else statement
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    bool data_available = false;
    while (data_available)
        process_data(); /* Non-compliant */

    if (flag_1)
        if (flag_2)     /* Non-compliant */
            action_1(); /* Non-compliant */
        else            /* Non-compliant */
            action_2();

    if (flag_1)
    {
        action_1();
    }
    else if (flag_2) /* Compliant by exception */
    {
        action_2();
    }
    else
    {
        ;
    }

    while (flag)
        ; /* Non-compliant */
    {
        flag = fn();
    }

    while (!data_available) /* Compliant - a loop with an empty body */
    {
    }

    if (flag_1)
    {
        action_1();
    }
    else if (flag_2)
    {
        action_2();
    } /* Non-compliant */

    if (flag_1)
    {
        action_1();
    }
    else if (flag_2)
    {
        action_2();
    }
    else
    {
        ; /* No action required - ; is optional */
    }

    return 0;
}