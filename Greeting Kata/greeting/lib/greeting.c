#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <greeting.h>

#define HELLOU "HELLO "
#define HELLOL "Hello, "
#define HELLO_MY_FRIEND "Hello, my friend."

static bool is_uppercase(const char *name);
static int8_t extract_names(const char *name);

static char names[MAX_NAMES][NAME_MAX_LENGTH + 1] = {};
static char output[MAX_NAMES * (NAME_MAX_LENGTH + 5U)] = {};

char *greet(const char *name)
{
    int8_t number_of_names = extract_names(name);

    if (number_of_names == -1)
    {
        return NULL;
    }

    if (number_of_names == 0)
    {
        return HELLO_MY_FRIEND;
    }

    uint8_t uppers = 0U, lowers = 0U;
    memset(output, 0, sizeof(output));
    for (uint8_t i = 0; i < number_of_names; i++)
    {
        if (is_uppercase(names[i]))
        {
            uppers++;
        }
        else
        {
            lowers++;
        }
    }

    for (uint8_t i = 0, j = 0; (i < number_of_names) && (j < lowers); i++)
    {
        if (!is_uppercase(names[i]))
        {
            if (j == 0)
            {
                sprintf(output, "%s%s", HELLOL, names[i]);
            }
            else if (j < lowers - 1)
            {
                strcat(output, ", ");
                strcat(output, names[i]);
            }
            else
            {
                strcat(output, (lowers > 2) ? ", and " : " and ");
                strcat(output, names[i]);
            }

            j++;

            if (j == lowers)
            {
                strcat(output, ".");
            }
        }
    }

    if (lowers && uppers)
    {
        strcat(output, " AND ");
    }

    for (uint8_t i = 0, j = 0; (i < number_of_names) && (j < uppers); i++)
    {
        if (is_uppercase(names[i]))
        {
            if (j == 0)
            {
                strcat(output, HELLOU);
                strcat(output, names[i]);
            }
            else if (j < uppers - 1)
            {
                strcat(output, ", ");
                strcat(output, names[i]);
            }
            else
            {
                strcat(output, (uppers > 2) ? ", AND " : " AND ");
                strcat(output, names[i]);
            }

            j++;

            if (j == uppers)
            {
                strcat(output, "!");
            }
        }
    }

    return output;
}

static int8_t extract_names(const char *name)
{
    uint8_t i = 0;
    int8_t num = 0U;
    memset(names, 0, sizeof(names));

    if (name == NULL)
    {
        return num;
    }

    for (char *ptr = (char *)name; *ptr != 0; ptr++)
    {
        if (*ptr == ',')
        {
            i = 0;
            if (strlen(names[num]))
            {
                if (num == MAX_NAMES)
                {
                    return -1;
                }
                num++;
            }
        }
        else if (isalpha(*ptr))
        {
            if (i == NAME_MAX_LENGTH)
            {
                return -1;
            }

            names[num][i] = *ptr;
            i++;
        }
    }

    if (strlen(names[num]))
    {
        num++;
    }

    return num;
}

static bool is_uppercase(const char *name)
{
    for (char *ptr = (char *)name; *ptr != 0; ptr++)
    {
        if (islower(*ptr))
        {
            return false;
        }
    }
    return true;
}