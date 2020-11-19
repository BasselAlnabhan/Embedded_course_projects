/**
 * There shall be no attempt to write to a stream which has been opened as read-only
 * 
 * The Standard does not specify the behaviour if an attempt is made to write to a read-only stream. 
 * For this reason it is considered unsafe to write to a read-only stream.
 */

#include <stdio.h>
#include <stdint.h>

void fn(void)
{
    FILE *fp = fopen("tmp", "r");

    (void)fprintf(fp, "What happens now?"); /* Non-compliant */

    (void)fclose(fp);
}

int main()
{
    return 0;
}