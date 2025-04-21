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
    size_t nb_chars = 0;
    size_t size     = ARRAY_LEN;

    value_link_t *str_link = a->head;
    while (str_link)
    {
        rw_value_t *values = str_link->values;
        ADD_VALUES_FOR_MODE(
            rw_value_t, get_rw_array_as_str, get_rw_dict_as_str
        );

        // Adds the length of each string in the linked list
        string_link_t *link = ll->head;
        while (link)
        {
            nb_chars += link->s.len;
            link = link->next;
        }
        str_link = str_link->next;
    }
    // size - 1 because we have a comma for each value except the last one
    // size + 1 because we have 1 line return for each value + for '[' */ /*
    // size * indent * 4 because we have 4 spaces of indent for each value
    // => nb += size - 1 + size + 1 + size * indent * 4;
    return nb_chars + (a->size * 2) + (a->size * indent * 4);
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
    size_t nb_chars = 0;
    size_t size     = ARRAY_LEN;

    item_link_t *str_link = d->head;
    while (str_link)
    {
        rw_item_t *items = str_link->items;
        ADD_ITEMS_FOR_MODE(rw_item_t, get_rw_array_as_str, get_rw_dict_as_str);

        // Adds the length of each string in the linked list
        string_link_t *link = ll->head;
        while (link)
        {
            nb_chars += link->s.len;
            link = link->next;
        }
        str_link = str_link->next;
    }
    // size * 4 because each key has 2 '"', and we add ': ' after each key
    // size - 1 because we have a comma for each item except the last one
    // size + 1 because we have 1 line return for each item + for '{'
    // size * indent * 4 because we have 4 spaces of indent for each item
    // => nb += size * 4 + size - 1 + size + 1 + size * indent * 4;
    return nb_chars + (d->size * 6) + (d->size * indent * 4);
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_rw_array_as_str(rw_array_t *a, u16 indent)
{
    if (!a)
    {
        return NULL_STRING;
    }
    ARRAY_AS_STR(fill_rw_string_ll_with_values);
}

string_t get_rw_dict_as_str(rw_dict_t *d, u16 indent)
{
    if (!d)
    {
        return NULL_STRING;
    }
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
    string_t s = get_rw_array_as_str(&a, 1);
    printf("%s", s.str);
    destroy_string(s);
#endif
}

void rw_dict_print(rw_dict_t d)
{
#ifndef VALGRING_DISABLE_PRINT
    string_t s = get_rw_dict_as_str(&d, 1);
    printf("%s", s.str);
    destroy_string(s);
#endif
}

void rw_json_print(rw_json_t j)
{
#ifndef VALGRING_DISABLE_PRINT
    if (j.is_array && j.array)
    {
        rw_array_print(*j.array);
    }
    else if (!j.is_array && j.dict)
    {
        rw_dict_print(*j.dict);
    }
#endif
}
