/**
 * The value of a pointer to a FILE shall not be used after the associated stream has been closed
 * 
 * The Standard states that the value of a FILE pointer is indeterminate after a close operation on a stream
 */

#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *fp;
    void *p;

    fp = fopen("tmp", "w");
    if (fp == NULL)
    {
        error_action();
    }
    fclose(fp);

    fprintf(fp, "?"); /* Non-compliant */
    p = fp;           /* Non-compliant */

    return 0;
}