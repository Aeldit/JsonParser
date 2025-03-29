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
    size_t total_size  = size;
    ADD_VALUES_FOR_MODE(ro_value_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
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
    // 'String's + counts the number of chars required for each value
    ro_item_t *items  = d.items;
    size_t size       = d.size;
    size_t total_size = size;
    ADD_ITEMS_FOR_MODE(ro_item_t, get_ro_array_as_str, get_ro_dict_as_str);
    return nb;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_ro_array_as_str(ro_array_t a, u16 indent)
{
    if (!a.size)
    {
        char *str = malloc(3 * sizeof(char));
        if (!str)
        {
            return NULL_STRING;
        }
        memcpy(str, "[]", 2);
        str[2] = 0;
        return STRING_OF(str, 2);
    }
    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));
    if (!ll)
    {
        return NULL_STRING;
    }                                                                          \
    /* '[' + '\n' + (indent - 1) * 4 * ' ' + ']' + '\n'*/                      \
    /* indents are 4 spaces */                                                 \
    /* indent == 1 -> if we are in the 'root' array, we add a '\n' at the */   \
    /* end */
    size_t nb_chars = 2 + ((indent - 1) * 4) + (indent == 1)
        + fill_ro_string_ll_with_values(ll, a, indent);
    u32 nb_chars_indent = indent * 4;
    char *str           = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_linked_list(ll);
        return NULL_STRING;
    } /* |-> Start building the string */
    str[0]              = '[';
    str[1]              = '\n';
    size_t insert_idx   = 2;
    string_link_t *link = ll->head;
    while (link)
    { /* Tabs */
        memset(str + insert_idx, ' ', nb_chars_indent);
        insert_idx += nb_chars_indent;
        if (link->is_from_str)
        {
            str[insert_idx++] = '"';
        } /* Value as string */
        memcpy(str + insert_idx, link->s.str, link->s.len);
        insert_idx += link->s.len;
        if (link->is_from_str)
        {
            str[insert_idx++] = '"';
        } /* Comma and line return */
        if (link->next)
        {
            str[insert_idx++] = ',';
        }
        str[insert_idx++] = '\n';
        link              = link->next;
    }
    if (indent == 1)
    {
        str[nb_chars - 2] = ']';
        str[nb_chars - 1] = '\n';
    }
    else
    { /* Tabs before the last ']' */
        memset(str + insert_idx, ' ', nb_chars_indent - 4);
        insert_idx += nb_chars_indent - 4;
        str[nb_chars - 1] = ']';
    } /* |-> End of string building */
    destroy_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

string_t get_ro_dict_as_str(ro_dict_t d, u16 indent)
{
    if (!d.size)
    {
        char *str = malloc(3 * sizeof(char));
        if (!str)
        {
            return NULL_STRING;
        }
        memcpy(str, "{}", 2);
        str[2] = 0;
        return STRING_OF(str, 2);
    }
    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));
    if (!ll)
    {
        return NULL_STRING;
    } /* '{' + '\n' + (indent - 1) * '\t' + '}' + '\n'*/ /* indent == 1 -> if we
                                                            are in the 'root'
                                                            dict, we add a \n at
                                                            the end */
    size_t nb_chars = 2 + ((indent - 1) * 4) + (indent == 1)
        + fill_ro_string_ll_with_items(ll, d, indent);
    u32 nb_chars_indent = indent * 4;
    char *str           = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_linked_list(ll);
        return NULL_STRING;
    } /* |-> Start building the string */
    str[0]              = '{';
    str[1]              = '\n';
    size_t insert_idx   = 2;
    u8 is_key           = 1;
    string_link_t *link = ll->head;
    while (link)
    {
        if (is_key)
        { /* Tabs */
            memset(str + insert_idx, ' ', nb_chars_indent);
            insert_idx += nb_chars_indent;
            str[insert_idx++] = '"'; /* String's contents */
            memcpy(str + insert_idx, link->s.str, link->s.len);
            insert_idx += link->s.len;
            memcpy(str + insert_idx, "\": ", 3);
            insert_idx += 3;
        }
        else
        {
            if (link->is_from_str)
            {
                str[insert_idx++] = '"';
            }
            memcpy(str + insert_idx, link->s.str, link->s.len);
            insert_idx += link->s.len;
            if (link->is_from_str)
            {
                str[insert_idx++] = '"';
            } /* Comma and line return */
            if (link->next)
            {
                str[insert_idx++] = ',';
            }
            str[insert_idx++] = '\n';
        }
        is_key = !is_key;
        link   = link->next;
    }
    if (indent == 1)
    {
        str[nb_chars - 2] = '}';
        str[nb_chars - 1] = '\n';
    }
    else
    { /* Tabs before the last '}' */
        memset(str + insert_idx, ' ', nb_chars_indent - 4);
        insert_idx += nb_chars_indent - 4;
        str[nb_chars - 1] = '}';
    } /* |-> End of string building */
    destroy_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

/*******************************************************************************
**                                   WRITING                                  **
*******************************************************************************/
void write_ro_json_to_file(ro_json_t j, char *file_name)
{
    if (!file_name)
    {
        return;
    }
    FILE *f = fopen(file_name, "w");
    if (!f)
    {
        return;
    }
    string_t s = j.is_array ? get_ro_array_as_str(j.array, 1)
                            : get_ro_dict_as_str(j.dict, 1);
    if (!s.str)
    {
        fclose(f);
        return;
    }
    fwrite(s.str, sizeof(char), s.len, f);
    free(s.str);
    fclose(f);
}

void ro_array_print(ro_array_t a)
{
#ifndef VALGRING_DISABLE_PRINT
    printf("%s\n", get_ro_array_as_str(a, 1).str);
#endif
}

void ro_dict_print(ro_dict_t d)
{
#ifndef VALGRING_DISABLE_PRINT
    printf("%s\n", get_ro_dict_as_str(d, 1).str);
#endif
}
