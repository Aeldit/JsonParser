#include "ro_json_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t get_int_as_str(int value);
string_t get_double_as_str(double value);
string_t get_bool_as_str(char value);
string_t get_null_as_str();
string_t get_array_as_str(ro_array_t *a, unsigned indent);
string_t get_dict_as_str(ro_dict_t *d, unsigned indent);

void add_link(string_linked_list_t *ll, string_t str, char str_needs_free,
              char is_from_str)
{
    if (!ll)
    {
        return;
    }

    string_link_t *sl = calloc(1, sizeof(string_link_t));
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

void destroy_linked_list(string_linked_list_t *ll)
{
    if (!ll)
    {
        return;
    }

    string_link_t *link = ll->head;
    while (link)
    {
        string_link_t *tmp = link;
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
unsigned handle_values(string_linked_list_t *ll, ro_value_t *values,
                       unsigned size, unsigned total_size, unsigned indent)
{
    unsigned nb_chars = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_value_t v = values[i];
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
            tmp_str = get_int_as_str(v.intv);
            break;
        case T_DOUBLE:
            tmp_str = get_double_as_str(v.doublev);
            break;
        case T_BOOL:
            tmp_str = get_bool_as_str(v.boolv);
            break;
        case T_NULL:
            tmp_str = get_null_as_str();
            break;
        case T_ARR:
            tmp_str = get_array_as_str(v.arrayv, indent + 1);
            break;
        case T_DICT:
            tmp_str = get_dict_as_str(v.dictv, indent + 1);
            break;
        }
        add_link(ll, tmp_str, v.type != T_STR, v.type == T_STR);
        // We add 1 for the comma if we are not at the last value
        // We add 1 for the line return
        // We add 'indent' for the tabs
        nb_chars += tmp_str.length + (i < total_size ? 1 : 0) + 1 + indent * 4;
    }
    return nb_chars;
}

unsigned handle_items(string_linked_list_t *ll, ro_item_t *items, unsigned size,
                      unsigned total_size, unsigned indent)
{
    unsigned nb_chars = 0;
    char is_key = 1;
    for (unsigned i = 0; i < size; ++i)
    {
        ro_item_t it = items[i];
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
            tmp_str = get_int_as_str(it.intv);
            break;
        case T_DOUBLE:
            tmp_str = get_double_as_str(it.doublev);
            break;
        case T_BOOL:
            tmp_str = get_bool_as_str(it.boolv);
            break;
        case T_NULL:
            tmp_str = get_null_as_str();
            break;
        case T_ARR:
            tmp_str = get_array_as_str(it.arrayv, indent + 1);
            break;
        case T_DICT:
            tmp_str = get_dict_as_str(it.dictv, indent + 1);
            break;
        }
        add_link(ll, it.key, 0, 1);
        // + 4 because we add '": "' after the key
        nb_chars += it.key.length + 4;

        add_link(ll, tmp_str, it.type != T_STR, it.type == T_STR);
        // We add 1 for the comma if we are not at the last value
        // We add 1 for the line return
        // We add 'indent' for the tabs
        nb_chars += tmp_str.length + (i < total_size ? 1 : 0) + 1 + indent * 4;
        is_key = !is_key;
    }
    return nb_chars;
}

/**
** \returns The number of additional characters
*/
unsigned fill_string_ll_with_values(string_linked_list_t *ll, ro_array_t *a,
                                    unsigned indent)
{
    if (!ll || !a)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    return handle_values(ll, a->values, a->size, a->size, indent);
}

/**
** \returns The number of additional characters
*/
unsigned fill_string_ll_with_items(string_linked_list_t *ll, ro_dict_t *d,
                                   unsigned indent)
{
    if (!ll || !d)
    {
        return 0;
    }

    // Iterates over each value of the array and converts them to
    // 'String's + counts the number of chars required for each value
    return handle_items(ll, d->items, d->size, d->size, indent);
}

/*******************************************************************************
**                                GETS AS STR                                 **
*******************************************************************************/
string_t get_int_as_str(int value)
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

string_t get_double_as_str(double value)
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

string_t get_bool_as_str(char value)
{
    char nb_chars = (value ? 4 : 5);
    char *str = calloc(nb_chars + 1, sizeof(char));
    memcpy(str, value ? "true" : "false", nb_chars);
    return STRING_OF(str ? str : 0, str ? nb_chars : 0);
}

string_t get_null_as_str()
{
    char *str = calloc(5, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }
    memcpy(str, "null", 4);
    return STRING_OF(str, 4);
}

string_t get_array_as_str(ro_array_t *a, unsigned indent)
{
    if (!a)
    {
        return EMPTY_STRING;
    }

    if (!a->size)
    {
        char *str = calloc(3, sizeof(char));
        if (!str)
        {
            return EMPTY_STRING;
        }
        memcpy(str, "[]", 2);
        return STRING_OF(str, 2);
    }

    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));
    if (!ll)
    {
        return EMPTY_STRING;
    }

    // '[' + '\n' + (indent - 1) * 4 * ' ' + ']' + '\n'
    // indents are 4 spaces
    // indent == 1 -> if we are in the 'root' array, we add a '\n' at the
    // end
    unsigned nb_chars = 2 + (indent - 1) * 4 + (indent == 1)
        + fill_string_ll_with_values(ll, a, indent);
    unsigned nb_chars_indent = indent * 4;

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_linked_list(ll);
        return EMPTY_STRING;
    }

    // |-> Start building the string
    str[0] = '[';
    str[1] = '\n';
    unsigned insert_idx = 2;

    string_link_t *link = ll->head;
    while (link)
    {
        // Tabs
        memset(str + insert_idx, ' ', nb_chars_indent);
        insert_idx += nb_chars_indent;

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
        memset(str + insert_idx, ' ', nb_chars_indent - 4);
        insert_idx += nb_chars_indent - 4;

        str[nb_chars - 1] = ']';
    }
    // |-> End of string building

    destroy_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

string_t get_dict_as_str(ro_dict_t *d, unsigned indent)
{
    if (!d)
    {
        return EMPTY_STRING;
    }

    if (!d->size)
    {
        char *str = calloc(3, sizeof(char));
        if (!str)
        {
            return EMPTY_STRING;
        }
        memcpy(str, "{}", 2);
        return STRING_OF(str, 2);
    }

    string_linked_list_t *ll = calloc(1, sizeof(string_linked_list_t));
    if (!ll)
    {
        return EMPTY_STRING;
    }

    // '{' + '\n' + (indent - 1) * 4 * ' ' + '}' + '\n'
    // indent == 1 -> if we are in the 'root' dict, we add a '\n' at the end
    unsigned nb_chars = 2 + (indent - 1) * 4 + (indent == 1)
        + fill_string_ll_with_items(ll, d, indent);
    unsigned nb_chars_indent = indent * 4;

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        destroy_linked_list(ll);
        return EMPTY_STRING;
    }

    // |-> Start building the string
    str[0] = '{';
    str[1] = '\n';
    unsigned insert_idx = 2;

    char is_key = 1;
    string_link_t *link = ll->head;
    while (link)
    {
        if (is_key)
        {
            // Tabs
            memset(str + insert_idx, ' ', nb_chars_indent);
            insert_idx += nb_chars_indent;

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
        // Tabs before the last '}'
        memset(str + insert_idx, ' ', nb_chars_indent - 4);
        insert_idx += nb_chars_indent - 4;

        str[nb_chars - 1] = '}';
    }
    // |-> End of string building

    destroy_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

/*******************************************************************************
**                                   WRITING **
*******************************************************************************/
void write_ro_json_to_file(ro_json_t *j, char *file_name)
{
    if (!j || !file_name)
    {
        return;
    }

    FILE *f = fopen(file_name, "w");
    if (!f)
    {
        return;
    }

    string_t s = IS_ARRAY(j) ? get_array_as_str(j->array, 1)
                             : get_dict_as_str(j->dict, 1);
    if (!s.str)
    {
        fclose(f);
        return;
    }

    printf("%s", s.str);
    /*printf("\n");
    for (unsigned i = 0; i < s.length; ++i)
    {
        printf("%c", s.str[i] ? s.str[i] : '0');
    }*/

    fwrite(s.str, sizeof(char), s.length, f);
    free(s.str);
    fclose(f);
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
