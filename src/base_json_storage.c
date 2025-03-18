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

bool strings_equals(string_t s1, string_t s2)
{
    size_t length = s1.len;
    if (length != s2.len)
    {
        return false;
    }

    const char *a = s1.str;
    const char *b = s2.str;
    if (!a || !b)
    {
        return false;
    }

    for (size_t i = 0; i < length; ++i)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

inline bool exp_long_equals(exp_long_t a, exp_long_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

inline bool exp_double_equals(exp_double_t a, exp_double_t b)
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
