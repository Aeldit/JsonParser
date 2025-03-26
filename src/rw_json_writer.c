#include "rw_json_writer.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_writer.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_rw_array_as_str(rw_array_t *a, u16 indent);
string_t get_rw_dict_as_str(rw_dict_t *d, u16 indent);

/**
** \returns The number of additional characters
*/
size_t fill_rw_string_ll_with_values(
    string_linked_list_t *ll, rw_array_t *a, u16 indent
)
{
    if (!ll || !a)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    size_t nb_chars    = 0;
    value_link_t *link = a->head;
    while (link)
    {
        rw_value_t *values = link->values;
        arr_size_t size    = ARRAY_LEN;
        size_t total_size  = a->size;
        ADD_VALUES_FOR_MODE(
            rw_value_t, get_rw_array_as_str, get_rw_dict_as_str
        );
        nb_chars += nb;
        link = link->next;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
size_t
fill_rw_string_ll_with_items(string_linked_list_t *ll, rw_dict_t *d, u16 indent)
{
    if (!ll || !d)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    size_t nb_chars   = 0;
    item_link_t *link = d->head;
    while (link)
    {
        rw_item_t *items  = link->items;
        arr_size_t size   = ARRAY_LEN;
        size_t total_size = d->size;
        ADD_ITEMS_FOR_MODE(rw_item_t, get_rw_array_as_str, get_rw_dict_as_str);
        nb_chars += nb;
        link = link->next;
    }
    return nb_chars;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_rw_array_as_str(rw_array_t *a, u16 indent)
{
    if (a)
    {
        GET_ARRAY_AS_STR(fill_rw_string_ll_with_values);
    }
    return NULL_STRING;
}

string_t get_rw_dict_as_str(rw_dict_t *d, u16 indent)
{
    if (d)
    {
        GET_DICT_AS_STR(fill_rw_string_ll_with_items);
    }
    return NULL_STRING;
}

/*******************************************************************************
**                                   WRITING **
*******************************************************************************/
void write_rw_json_to_file(rw_json_t *j, char *file_name)
{
    WRITE_JSON_TO_FILE(get_rw_array_as_str, get_rw_dict_as_str);
}
