#include "json_api.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "lists/linked_lists.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
typed_value_st get_value(json_dict_st *jd, char *key, size_t key_len)
{
    if (jd == NULL || key == NULL)
    {
        return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
    }

    // Iterates over the items
    struct item_link *tmp = jd->items->head;
    while (tmp != NULL)
    {
        // Iterates over the keys
        size_t len = tmp->next == NULL ? jd->items->idx : ARRAY_LEN;
        for (size_t j = 0; j < len; ++j)
        {
            if (!strncmp(tmp->items[j]->key, key, key_len))
            {
                return (typed_value_st){ .type = tmp->items[j]->type,
                                         .value = tmp->items[j]->value };
            }
        }
        tmp = tmp->next;
    }
    return (typed_value_st){ .type = TYPE_ERROR, .value = NULL };
}

void destroy_dict(json_dict_st *jd)
{
    if (jd == NULL)
    {
        return;
    }

    destroy_item_control(jd->items);
    destroy_key_control(jd->keys);

    destroy_str_control(jd->strings);
    destroy_num_control(jd->numbers);
    destroy_json_dict_control(jd->json_dicts);
    destroy_array_control(jd->lists);
    destroy_bool_control(jd->booleans);
    free(jd);
}
