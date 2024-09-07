#include "json_api.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
char strings_equal(const char *a, uint64_t len_a, const char *b, uint64_t len_b)
{
    if (a == NULL || b == NULL)
    {
        return 0;
    }

    if (len_a != len_b)
    {
        return 0;
    }

    for (uint64_t i = 0; i < len_a; ++i)
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
json_st parse(char *file)
{
    FILE *f = fopen(file, "r");
    json_st j = { 0 };
    j.storage = calloc(1, sizeof(storage_st));
    if (f == NULL)
    {
        return j;
    }

    uint64_t offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        fclose(f);
        return j;
    }

    char c = fgetc(f);
    if (c == '{')
    {
        j.is_array = 0;
        j.jd = parse_json_dict(j.storage, f, &offset);
    }
    else if (c == '[')
    {
        j.is_array = 1;
        j.ja = parse_array(j.storage, f, &offset);
    }
    fclose(f);
    return j;
}

typed_value_st get_value_at(json_array_st *ja, uint64_t index)
{
    if (ja == NULL || index >= ja->size)
    {
        return (typed_value_st){ .value = NULL, .type = TYPE_ERROR };
    }

    // Iterates over the items
    uint64_t current_index = 0;
    struct link_value *link = ja->head;
    while (link != NULL)
    {
        for (uint64_t i = 0; i < BASE_ARRAY_LEN; ++i, ++current_index)
        {
            if (current_index == index)
            {
                return link->elts[i];
            }
        }
        link = link->next;
    }
    return (typed_value_st){ .value = NULL, .type = TYPE_ERROR };
}

item_st get_item(json_dict_st *jd, char *key, uint64_t key_len)
{
    if (jd == NULL)
    {
        return (item_st){ .key = NULL, .value = NULL, .type = TYPE_ERROR };
    }

    // Iterates over the items
    struct link_item *link = jd->head;
    while (link != NULL)
    {
        for (uint64_t i = 0; i < BASE_ARRAY_LEN; ++i)
        {
            item_st it = link->elts[i];
            if (strings_equal(key, key_len, it.key, it.key_len))
            {
                return link->elts[i];
            }
        }
        link = link->next;
    }
    return (item_st){ .key = NULL, .value = NULL, .type = TYPE_ERROR };
}

void destroy_json(json_st *j)
{
    if (j == NULL)
    {
        return;
    }
    destroy_json_array(j->ja);
    destroy_json_dict(j->jd);
    destroy_storage(j->storage);
}
