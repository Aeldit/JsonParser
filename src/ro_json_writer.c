#include "ro_json_writer.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_writer.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t *a, unsigned indent);
string_t get_ro_dict_as_str(ro_dict_t *d, unsigned indent);

/**
** \returns The number of additional characters
*/
unsigned fill_ro_string_ll_with_values(string_linked_list_t *ll, ro_array_t *a,
                                       unsigned indent)
{
    if (!ll || !a)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    ro_value_t *values = a->values;
    unsigned size = a->size;
    unsigned total_size = size;
    ADD_VALUES_FOR_MODE(ro_value_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
}

/**
** \returns The number of additional characters
*/
unsigned fill_ro_string_ll_with_items(string_linked_list_t *ll, ro_dict_t *d,
                                      unsigned indent)
{
    if (!ll || !d)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    ro_item_t *items = d->items;
    unsigned size = d->size;
    unsigned total_size = size;
    ADD_ITEMS_FOR_MODE(ro_item_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t *a, unsigned indent)
{
    GET_ARRAY_AS_STR(fill_ro_string_ll_with_values);
}

string_t get_ro_dict_as_str(ro_dict_t *d, unsigned indent)
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
