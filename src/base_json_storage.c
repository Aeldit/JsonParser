#include "base_json_storage.h"

/*******************************************************************************
**                                  FUNCTIONS                                 **
*******************************************************************************/
char strings_equals(string_t s1, string_t s2)
{
    unsigned length = s1.len;
    if (length != s2.len)
    {
        return 0;
    }

    const char *a = s1.str;
    const char *b = s2.str;
    if (!a || !b)
    {
        return 0;
    }

    for (unsigned i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
    }
    return 1;
}
