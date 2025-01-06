#include "base_json_storage.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
**                                  FUNCTIONS                                 **
*******************************************************************************/
inline string_t string_of(char *s)
{
    return s ? (string_t){ .str = s, .len = strlen(s), .needs_freeing = 1 }
             : NULL_STRING;
}

inline string_t string_nofree_of(char *s)
{
    return s ? (string_t){ .str = s, .len = strlen(s), .needs_freeing = 0 }
             : NULL_STRING;
}

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

void destroy_string(string_t s)
{
    if (s.str && s.needs_freeing)
    {
        free(s.str);
    }
}
