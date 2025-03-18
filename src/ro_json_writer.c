#include "ro_json_writer.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_writer.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t *a, u16 indent);
string_t get_ro_dict_as_str(ro_dict_t *d, u16 indent);

/**
** \returns The number of additional characters
*/
size_t fill_ro_string_ll_with_values(string_linked_list_t *ll, ro_array_t *a,
                                     u16 indent)
{
    if (!ll || !a)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    ro_value_t *values = a->values;
    size_t size = a->size;
    size_t total_size = size;
    ADD_VALUES_FOR_MODE(ro_value_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
}

/**
** \returns The number of additional characters
*/
unsigned fill_ro_string_ll_with_items(string_linked_list_t *ll, ro_dict_t *d,
                                      u16 indent)
{
    if (!ll || !d)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    ro_item_t *items = d->items;
    size_t size = d->size;
    size_t total_size = size;
    ADD_ITEMS_FOR_MODE(ro_item_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t *a, u16 indent)
{
    GET_ARRAY_AS_STR(fill_ro_string_ll_with_values);
}

string_t get_ro_dict_as_str(ro_dict_t *d, u16 indent)
{
    GET_DICT_AS_STR(fill_ro_string_ll_with_items);
}

/*******************************************************************************
**                                   WRITING                                  **
*******************************************************************************/
void write_ro_json_to_file(ro_json_t *j, char *file_name)
{
    WRITE_JSON_TO_FILE(get_ro_array_as_str, get_ro_dict_as_str);
}
