#include "json_api.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
