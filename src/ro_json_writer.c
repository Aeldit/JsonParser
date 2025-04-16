#include "ro_json_writer.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_writer.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t a, u16 indent);
string_t get_ro_dict_as_str(ro_dict_t d, u16 indent);

/**
** \returns The number of additional characters
*/
size_t fill_ro_string_ll_with_values(
    string_linked_list_t *ll, ro_array_t a, u16 indent
)
{
    if (!ll)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    ro_value_t *values = a.values;
    size_t size        = a.size;
    size_t nb_chars    = 0;
    ADD_VALUES_FOR_MODE(ro_value_t, get_ro_array_as_str, get_ro_dict_as_str);

    // size - 1 because we have a comma for each value except the last one
    // size + 1 because we have 1 line return for each value + for '[' */ /*
    // size * indent * 4 because we have 4 spaces of indent for each value
    // => nb += size - 1 + size + 1 + size * indent * 4;
    nb_chars += size * 2 + size * indent * 4;
    // Adds the length of each string in the linked list
    string_link_t *link = ll->head;
    while (link)
    {
        nb_chars += link->s.len;
        link = link->next;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
unsigned
fill_ro_string_ll_with_items(string_linked_list_t *ll, ro_dict_t d, u16 indent)
{
    if (!ll)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's
    ro_item_t *items = d.items;
    size_t size      = d.size;
    size_t nb_chars  = 0;
    ADD_ITEMS_FOR_MODE(ro_item_t, get_ro_array_as_str, get_ro_dict_as_str);

    // size * 4 because we add '": "' after each key
    // size - 1 because we have a comma for each item except the last one
    // size + 1 because we have 1 line return for each item + for '{'
    // size * indent * 4 because we have 4 spaces of indent for each item
    // => nb += size * 4 + size - 1 + size + 1 + size * indent * 4;
    nb_chars += size * 6 + size * indent * 4;
    // Adds the length of each string in the linked list
    string_link_t *link = ll->head;
    while (link)
    {
        nb_chars += link->s.len;
        link = link->next;
    }
    return nb_chars;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t a, u16 indent)
{
    ARRAY_AS_STR(fill_ro_string_ll_with_values);
}

string_t get_ro_dict_as_str(ro_dict_t d, u16 indent)
{
    DICT_AS_STR(fill_ro_string_ll_with_items);
}

/*******************************************************************************
**                                   WRITING                                  **
*******************************************************************************/
void write_ro_json_to_file(ro_json_t j, char *file_name)
{
    WRITE_JSON_TO_FILE(get_ro_array_as_str, get_ro_dict_as_str);
}

void ro_array_print(ro_array_t a)
{
#ifndef VALGRING_DISABLE_PRINT
    string_t s = get_ro_array_as_str(a, 1);
    printf("%s\n", s.str);
    destroy_string(s);
#endif
}

void ro_dict_print(ro_dict_t d)
{
#ifndef VALGRING_DISABLE_PRINT
    string_t s = get_ro_dict_as_str(d, 1);
    printf("%s\n", s.str);
    destroy_string(s);
#endif
}

void ro_json_print(ro_json_t j)
{
#ifndef VALGRING_DISABLE_PRINT
    if (j.is_array)
    {
        ro_array_print(j.array);
    }
    else
    {
        ro_dict_print(j.dict);
    }
#endif
}
