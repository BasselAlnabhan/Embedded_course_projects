#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define LENGTH(arr) (sizeof(arr) / sizeof(*arr))

typedef bool (*compare_t)(uint8_t, uint8_t);

static void swap(uint8_t *a, uint8_t *b);
static bool compareASC(uint8_t a, uint8_t b);
static bool compareDESC(uint8_t a, uint8_t b);
static void print_array(uint8_t *arr, uint8_t length);
static void sort_array(uint8_t *arr, uint8_t length, compare_t compare);

int main(void)
{
    uint8_t array[] = {23, 1, 7, 45, 0, 7, 11, 4, 90, 5};

    printf("Original Array   : ");
    print_array(array, LENGTH(array));

    printf("Sorted Aascending: ");
    sort_array(array, LENGTH(array), compareASC);
    print_array(array, LENGTH(array));

    printf("Sorted Descending: ");
    sort_array(array, LENGTH(array), compareDESC);
    print_array(array, LENGTH(array));

    return 0;
}

static void sort_array(uint8_t *arr, uint8_t length, compare_t compare)
{
    for (uint8_t i = 0; i < length; i++)
    {
        for (uint8_t j = i + 1; j < length; j++)
        {
            if (compare(arr[i], arr[j]))
            {
                swap(arr + i, arr + j);
            }
        }
    }
}

static void print_array(uint8_t *arr, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static void swap(uint8_t *a, uint8_t *b)
{
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

static bool compareASC(uint8_t a, uint8_t b)
{
    return (a > b);
}

static bool compareDESC(uint8_t a, uint8_t b)
{
    return (a < b);
}