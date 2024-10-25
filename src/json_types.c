#include "json_types.h"

#include <stdio.h>

char strings_equals(struct string s1, struct string s2)
{
    uint_strlen_t length = s1.length;
    if (length != s2.length)
    {
        return 0;
    }

    const char *a = s1.str;
    const char *b = s2.str;
    if (!a || !b)
    {
        return 0;
    }

    for (uint_strlen_t i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
    }
    return 1;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void print_err_bits(uint_fast16_t err)
{
    printf("%d : ERR_FSEEK\n%d : ERR_NULL_KEY\n%d : ERR_NULL_STR\n%d : "
           "ERR_NULL_ARR\n%d : ERR_NULL_DICT\n%d : ERR_ITEM_EXISTS\n%d : "
           "ERR_MAX_NESTED_ARRAYS_REACHED\n%d : ERR_MAX_NESTED_DICTS_REACHED\n"
           "%d : ERR_NULL_VALUE\n%d : ERR_NULL_ITEM\n",
           (ERR_FSEEK & err ? 1 : 0), (ERR_NULL_KEY & err ? 1 : 0),
           (ERR_NULL_STR & err ? 1 : 0), (ERR_NULL_ARR & err ? 1 : 0),
           (ERR_NULL_DICT & err ? 1 : 0), (ERR_ITEM_EXISTS & err ? 1 : 0),
           (ERR_MAX_NESTED_ARRAYS_REACHED & err ? 1 : 0),
           (ERR_MAX_NESTED_DICTS_REACHED & err ? 1 : 0),
           (ERR_NULL_VALUE & err ? 1 : 0), (ERR_NULL_ITEM & err ? 1 : 0));
}
