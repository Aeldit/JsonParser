#include "base_json_storage.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                  FUNCTIONS                                 **
*******************************************************************************/
inline string_t string_of(char *s)
{
    return s ? STRING_OF(s, strlen(s)) : NULL_STRING;
}

inline string_t string_nofree_of(char *s)
{
    return s ? STRING_NOFREE_OF(s, strlen(s)) : NULL_STRING;
}

char strings_equals(string_t s1, string_t s2)
{
    size_t length = s1.len;
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

    for (size_t i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
    }
    return 1;
}

inline char exp_long_equals(exponent_long_t a, exponent_long_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

inline char exp_double_equals(exponent_double_t a, exponent_double_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

void destroy_string(string_t s)
{
    if (s.str && s.needs_freeing)
    {
        free(s.str);
    }
}
