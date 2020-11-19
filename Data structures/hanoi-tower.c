#include <stdio.h>
#include <stdint.h>

#define DISKS 64
#define SECONDES_IN_YEAR (365 * 24 * 60 * 60)

uint64_t hanoi(uint8_t n);

int main()
{
    uint64_t years = hanoi(DISKS) / SECONDES_IN_YEAR;
    printf("\nIt takes approx. %llu years to move %d disks.\n\n", years, DISKS);
    return 0;
}

uint64_t hanoi(uint8_t n)
{
    return (n == 1) ? 1 : (2 * hanoi(n - 1) + 1);
}
