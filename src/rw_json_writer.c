#include "rw_json_writer.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_writer.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_rw_array_as_str(rw_array_t a, u16 indent);
string_t get_rw_dict_as_str(rw_dict_t d, u16 indent);

/**
** \returns The number of additional characters
*/
size_t fill_rw_string_ll_with_values(
    string_linked_list_t *ll, rw_array_t a, u16 indent
)
{
    if (!ll)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    size_t nb_chars        = 0;
    value_link_t *str_link = a.head;
    while (str_link)
    {
        rw_value_t *values = str_link->values;
        arr_size_t size    = ARRAY_LEN;
        ADD_VALUES_FOR_MODE(
            rw_value_t, get_rw_array_as_str, get_rw_dict_as_str
        );
        nb_chars += nb;
        str_link = str_link->next;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
size_t
fill_rw_string_ll_with_items(string_linked_list_t *ll, rw_dict_t d, u16 indent)
{
    if (!ll)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    size_t nb_chars       = 0;
    item_link_t *str_link = d.head;
    while (str_link)
    {
        rw_item_t *items = str_link->items;
        arr_size_t size  = ARRAY_LEN;
        ADD_ITEMS_FOR_MODE(rw_item_t, get_rw_array_as_str, get_rw_dict_as_str);
        nb_chars += nb;
        str_link = str_link->next;
    }
    return nb_chars;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_rw_array_as_str(rw_array_t a, u16 indent)
{
    ARRAY_AS_STR(fill_rw_string_ll_with_values);
}

string_t get_rw_dict_as_str(rw_dict_t d, u16 indent)
{
    DICT_AS_STR(fill_rw_string_ll_with_items);
}

/*******************************************************************************
**                                   WRITING                                  **
*******************************************************************************/
void write_rw_json_to_file(rw_json_t j, char *file_name)
{
    WRITE_JSON_TO_FILE(get_rw_array_as_str, get_rw_dict_as_str);
}

void rw_array_print(rw_array_t a)
{
#ifndef VALGRING_DISABLE_PRINT
    string_t s = get_rw_array_as_str(a, 1);
    printf("%s\n", s.str);
    destroy_string(s);
#endif
}

void rw_dict_print(rw_dict_t d)
{
#ifndef VALGRING_DISABLE_PRINT
    string_t s = get_rw_dict_as_str(d, 1);
    printf("%s\n", s.str);
    destroy_string(s);
#endif
}

void rw_json_print(rw_json_t j)
{
#ifndef VALGRING_DISABLE_PRINT
    if (j.is_array)
    {
        rw_array_print(j.array);
    }
    else
    {
        rw_dict_print(j.dict);
    }
#endif
}
