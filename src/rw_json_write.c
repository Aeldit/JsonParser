#include "rw_json_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t get_rw_int_as_str(int value);
string_t get_rw_double_as_str(double value);
string_t get_rw_bool_as_str(char value);
string_t get_rw_null_as_str();
string_t get_rw_array_as_str(rw_array_t *a, unsigned indent);
string_t get_rw_dict_as_str(rw_dict_t *d, unsigned indent);

void add_rw_link(rw_string_linked_list_t *ll, string_t str, char str_needs_free,
                 char is_from_str)
{
    if (!ll)
    {
        return;
    }

    rw_string_link_t *sl = calloc(1, sizeof(rw_string_link_t));
    if (!sl)
    {
        return;
    }
    sl->s = str;
    sl->s_needs_free = str_needs_free;
    sl->is_from_str = is_from_str;

    if (!ll->head)
    {
        ll->head = sl;
        ll->tail = sl;
    }
    else if (!ll->tail)
    {
        free(sl);
    }
    else
    {
        ll->tail->next = sl;
        ll->tail = sl;
    }
}

void destroy_rw_linked_list(rw_string_linked_list_t *ll)
{
    if (!ll)
    {
        return;
    }

    rw_string_link_t *link = ll->head;
    while (link)
    {
        rw_string_link_t *tmp = link;
        link = link->next;
        if (tmp->s_needs_free)
        {
            free(tmp->s.str);
        }
        free(tmp);
    }
    free(ll);
}

// FIX: Handle arrays that end with empty dicts
unsigned handle_rw_values(rw_string_linked_list_t *ll, rw_value_t *values,
                          unsigned size, unsigned total_size, unsigned indent)
{
    unsigned nb_chars = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        rw_value_t v = values[i];
        if (v.type == T_ERROR)
        {
            continue;
        }

        string_t tmp_str;
        switch (v.type)
        {
        case T_STR:
            tmp_str = v.strv;
            nb_chars += 2; // Strings are encased by 2 double-quotes (\"\")
            break;
        case T_INT:
            tmp_str = get_rw_int_as_str(v.intv);
            break;
        case T_DOUBLE:
            tmp_str = get_rw_double_as_str(v.doublev);
            break;
        case T_BOOL:
            tmp_str = get_rw_bool_as_str(v.boolv);
            break;
        case T_NULL:
            tmp_str = get_rw_null_as_str();
            break;
        case T_ARR:
            tmp_str = get_rw_array_as_str(v.arrayv, indent + 1);
            break;
        case T_DICT:
            tmp_str = get_rw_dict_as_str(v.dictv, indent + 1);
            break;
        }
        add_rw_link(ll, tmp_str, v.type != T_STR, v.type == T_STR);
        // We add 1 for the comma if we are not at the last value
        // We add 1 for the line return
        // We add 'indent' for the tabs
        // We add 2 if the item_t's value is a string (for the double quotes)
        nb_chars += tmp_str.length + (i == total_size - 1 ? 0 : 1) + 1 + indent
            + (v.type == T_STR ? 2 : 0);
    }
    return nb_chars;
}

unsigned handle_rw_items(rw_string_linked_list_t *ll, rw_item_t *items,
                         unsigned size, unsigned total_size, unsigned indent)
{
    unsigned nb_chars = 0;
    char is_key = 1;
    for (unsigned i = 0; i < size; ++i)
    {
        rw_item_t it = items[i];
        if (it.type == T_ERROR)
        {
            continue;
        }

        string_t tmp_str;
        switch (it.type)
        {
        case T_STR:
            tmp_str = it.strv;
            nb_chars += 2; // Strings are encased by 2 double-quotes (\"\")
            break;
        case T_INT:
            tmp_str = get_rw_int_as_str(it.intv);
            break;
        case T_DOUBLE:
            tmp_str = get_rw_double_as_str(it.doublev);
            break;
        case T_BOOL:
            tmp_str = get_rw_bool_as_str(it.boolv);
            break;
        case T_NULL:
            tmp_str = get_rw_null_as_str();
            break;
        case T_ARR:
            tmp_str = get_rw_array_as_str(it.arrayv, indent + 1);
            break;
        case T_DICT:
            tmp_str = get_rw_dict_as_str(it.dictv, indent + 1);
            break;
        }
        add_rw_link(ll, it.key, 0, 1);
        // + 2 because we add ": " after the key
        nb_chars += it.key.length + 4;

        add_rw_link(ll, tmp_str, it.type != T_STR, it.type == T_STR);
        // We add 1 for the comma if we are not at the last value
        // We add 1 for the line return
        // We add 'indent' for the tabs
        // We add 2 if the item_t's value is a string (for the double quotes)
        nb_chars += tmp_str.length + (i == total_size - 1 ? 0 : 1) + 1 + indent
            + (it.type == T_STR ? 2 : 0);
        is_key = !is_key;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
unsigned fill_rw_string_ll_with_values(rw_string_linked_list_t *ll,
                                       rw_array_t *a, unsigned indent)
{
    if (!ll || !a)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value

    unsigned nb_chars = 0;
    value_link_t *link = a->head;
    while (link)
    {
        nb_chars +=
            handle_rw_values(ll, link->values, ARRAY_LEN, a->size, indent);
        link = link->next;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
unsigned fill_rw_string_ll_with_items(rw_string_linked_list_t *ll, rw_dict_t *d,
                                      unsigned indent)
{
    if (!ll || !d)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value

    unsigned nb_chars = 0;
    item_link_t *link = d->head;
    while (link)
    {
        nb_chars +=
            handle_rw_items(ll, link->items, ARRAY_LEN, d->size, indent);
        link = link->next;
    }
    return nb_chars;
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_rw_int_as_str(int value)
{
    char is_neg = value < 0;
    unsigned nb_chars = is_neg ? 2 : 1;
    if (is_neg)
    {
        value *= -1;
    }

    int tmp_value = value;
    while (tmp_value /= 10)
    {
        ++nb_chars;
    }

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    string_t s = STRING_OF(str, nb_chars);

    while (nb_chars)
    {
        str[--nb_chars] = value % 10 + '0';
        value /= 10;
    }

    if (is_neg)
    {
        str[0] = '-';
    }
    return s;
}

string_t get_rw_double_as_str(double value)
{
    // 18 : 10 int digits + '.' + 6 floating point digits + '\0'
    char double_str[18];
    snprintf(double_str, 18, "%lf", value);

    int nb_chars = 0;
    int nb_decimals = 6;
    char is_in_decimals = 1;
    char non_zero_decimal_found = 0;
    for (int i = 17; i >= 0; --i)
    {
        char s = double_str[i];
        if (s == '.')
        {
            is_in_decimals = 0;
        }
        if (is_in_decimals && '1' <= s && s <= '9')
        {
            non_zero_decimal_found = 1;
        }
        if (s == '0' && is_in_decimals && !non_zero_decimal_found)
        {
            --nb_decimals;
        }
        if (!is_in_decimals)
        {
            ++nb_chars;
        }
    }

    char *str = calloc(18, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    string_t s = STRING_OF(str, nb_chars + nb_decimals - (nb_decimals ? 0 : 1));
    memcpy(str, double_str, s.length);
    return s;
}

string_t get_rw_bool_as_str(char value)
{
    char nb_chars = (value ? 4 : 5);
    char *str = calloc(nb_chars + 1, sizeof(char));
    memcpy(str, value ? "true" : "false", nb_chars);
    return STRING_OF(str ? str : 0, str ? nb_chars : 0);
}

string_t get_rw_null_as_str()
{
    char *str = calloc(5, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }
    memcpy(str, "null", 4);
    return STRING_OF(str, 4);
}

string_t get_rw_array_as_str(rw_array_t *a, unsigned indent)
{
    if (!a)
    {
        return EMPTY_STRING;
    }

    rw_string_linked_list_t *ll = calloc(1, sizeof(rw_string_linked_list_t));
    if (!ll)
    {
        return EMPTY_STRING;
    }

    // '[' + '\n' + (indent - 1) * '\t' + ']' + '\n'
    // indent == 1 -> if we are in the 'root' array, we add a '\n' at the
    // end
    unsigned nb_chars = indent - 1 + 3 + (indent == 1)
        + fill_rw_string_ll_with_values(ll, a, indent);

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_rw_linked_list(ll);
        return EMPTY_STRING;
    }

    // |-> Start building the string
    str[0] = '[';
    str[1] = '\n';
    unsigned insert_idx = 2;

    rw_string_link_t *link = ll->head;
    while (link)
    {
        // Tabs
        memset(str + insert_idx, '\t', indent);
        insert_idx += indent;

        if (link->is_from_str)
        {
            str[insert_idx++] = '"';
        }

        // Value as string
        memcpy(str + insert_idx, link->s.str, link->s.length);
        insert_idx += link->s.length;

        if (link->is_from_str)
        {
            str[insert_idx++] = '"';
        }

        // Comma and line return
        if (link->next)
        {
            str[insert_idx++] = ',';
        }
        str[insert_idx++] = '\n';

        link = link->next;
    }

    if (indent == 1)
    {
        str[nb_chars - 2] = ']';
        str[nb_chars - 1] = '\n';
    }
    else
    {
        // Tabs before the last ']'
        memset(str + insert_idx, '\t', indent - 1);
        insert_idx += indent - 1;

        str[nb_chars - 1] = ']';
    }
    // |-> End of string building

    destroy_rw_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

string_t get_rw_dict_as_str(rw_dict_t *d, unsigned indent)
{
    if (!d)
    {
        return EMPTY_STRING;
    }

    rw_string_linked_list_t *ll = calloc(1, sizeof(rw_string_linked_list_t));
    if (!ll)
    {
        return EMPTY_STRING;
    }

    // '{' + '\n' + (indent - 1) * '\t' + '}' + '\n'
    // indent == 1 -> if we are in the 'root' dict, we add a '\n' at the end
    unsigned nb_chars = indent - 1 + 3 + (indent == 1)
        + fill_rw_string_ll_with_items(ll, d, indent);

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_rw_linked_list(ll);
        return EMPTY_STRING;
    }

    // |-> Start building the string
    str[0] = '{';
    str[1] = '\n';
    unsigned insert_idx = 2;

    char is_key = 1;
    rw_string_link_t *link = ll->head;
    while (link)
    {
        if (is_key)
        {
            // Tabs
            memset(str + insert_idx, '\t', indent);
            insert_idx += indent;

            str[insert_idx++] = '"';

            // String's contents
            memcpy(str + insert_idx, link->s.str, link->s.length);
            insert_idx += link->s.length;

            memcpy(str + insert_idx, "\": ", 3);
            insert_idx += 3;
        }
        else
        {
            if (link->is_from_str)
            {
                str[insert_idx++] = '"';
            }

            memcpy(str + insert_idx, link->s.str, link->s.length);
            insert_idx += link->s.length;

            if (link->is_from_str)
            {
                str[insert_idx++] = '"';
            }

            // Comma and line return
            if (link->next)
            {
                str[insert_idx++] = ',';
            }
            str[insert_idx++] = '\n';
        }

        is_key = !is_key;
        link = link->next;
    }

    if (indent == 1)
    {
        str[nb_chars - 2] = '}';
        str[nb_chars - 1] = '\n';
    }
    else
    {
        // TODO: Use spaces instead of tabs
        // Tabs before the last '}'
        memset(str + insert_idx, '\t', indent - 1);
        insert_idx += indent - 1;

        str[nb_chars - 1] = '}';
    }
    // |-> End of string building

    destroy_rw_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

/*******************************************************************************
**                                   WRITING **
*******************************************************************************/
void write_rw_json_to_file(rw_json_t *j, char *file_name)
{
    if (!j || !file_name)
    {
        return;
    }

    if (j->is_array && j->array)
    {
        FILE *f = fopen(file_name, "w");
        if (!f)
        {
            return;
        }

        string_t s = get_rw_array_as_str(j->array, 1);
        if (!s.str)
        {
            fclose(f);
            return;
        }

        printf("%s", s.str);
        fwrite(s.str, sizeof(char), s.length, f);
        free(s.str);
        fclose(f);
    }
    else if (j->dict)
    {
        FILE *f = fopen(file_name, "w");
        if (!f)
        {
            return;
        }

        string_t s = get_rw_dict_as_str(j->dict, 1);
        if (!s.str)
        {
            fclose(f);
            return;
        }

        printf("%s", s.str);
        fwrite(s.str, sizeof(char), s.length, f);
        free(s.str);
        fclose(f);
    }
}

// FIX: Empty dicts or arrays
/*int main(void)
{
    Array *a = init_array(6);
    arr_add_int(a, 666);
    arr_add_double(a, 1234567891.100456);
    arr_add_bool(a, 1);
    arr_add_bool(a, 0);

    JSON *j = init_json(1, a, 0);

    write_json_to_file(j, "out.json");
    destroy_json(j);
    return 0;
}*/
