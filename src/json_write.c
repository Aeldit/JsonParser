#include "json_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_link(StringLinkedList *ll, String str)
{
    if (!ll)
    {
        return;
    }

    StringLink *sl = calloc(1, sizeof(StringLink));
    if (!sl)
    {
        return;
    }
    sl->s = str;

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

void destroy_linked_list(StringLinkedList *ll)
{
    if (!ll)
    {
        return;
    }

    StringLink *link = ll->head;
    while (link)
    {
        StringLink *tmp = link;
        link = link->next;
        free(tmp->s.str);
        free(tmp);
    }
    free(ll);
}

String get_int_as_str(int value)
{
    int nb_chars = 1;
    int tmp_value = value;
    while (tmp_value /= 10)
    {
        ++nb_chars;
    }

    unsigned char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    String s = STRING_OF(str, nb_chars);

    while (nb_chars)
    {
        str[--nb_chars] = value % 10 + '0';
        value /= 10;
    }
    return s;
}

String get_double_as_str(double value)
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

    unsigned char *str = calloc(18, sizeof(char));
    if (!str)
    {
        return EMPTY_STRING;
    }

    String s = STRING_OF(str, nb_chars + nb_decimals - (nb_decimals ? 0 : 1));
    memcpy(str, double_str, s.length);
    return s;
}

String get_bool_as_str(char value)
{
    char nb_chars = (value ? 4 : 5);
    unsigned char *str = calloc(nb_chars + 1, sizeof(char));
    memcpy(str, value ? "true" : "false", nb_chars);
    return STRING_OF(str ? str : 0, str ? nb_chars : 0);
}

String get_array_as_str(Array *a, unsigned indent)
{
    if (!a)
    {
        return EMPTY_STRING;
    }

    StringLinkedList *ll = calloc(1, sizeof(StringLinkedList));
    if (!ll)
    {
        return EMPTY_STRING;
    }

    // Iterates over each element of the array and converts them to
    // 'String's + counts the number of chars required for each value
    Value *values = a->values;
    unsigned size = a->size;
    unsigned nb_chars = 4; // [\n]\n
    for (unsigned i = 0; i < size; ++i)
    {
        Value v = values[i];
        String tmp_str;
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
        }
        add_link(ll, tmp_str);
        // We add 1 for the comma if we are not at the last value
        // We add 1 for the line return
        // We add 'indent' for the tabs
        nb_chars += tmp_str.length + (i == size - 1 ? 0 : 1) + 1 + indent;
    }

    unsigned char *str = calloc(nb_chars, sizeof(char));
    if (!str)
    {
        destroy_linked_list(ll);
        return EMPTY_STRING;
    }

    str[0] = '[';
    str[1] = '\n';
    unsigned insert_idx = 2;
    StringLink *link = ll->head;
    while (link)
    {
        // Tabs
        memset(str + insert_idx, '\t', indent);
        insert_idx += indent;

        // Value as string
        memcpy(str + insert_idx, link->s.str, link->s.length);
        insert_idx += link->s.length;

        // Comma and line return
        if (link->next)
        {
            str[insert_idx++] = ',';
        }
        str[insert_idx++] = '\n';

        link = link->next;
    }
    str[nb_chars - 2] = ']';
    printf("%s\n", str);
    destroy_linked_list(ll);
    return STRING_OF(str, nb_chars);
}

void write_json_to_file(JSON *j, char *file_name)
{
    if (!j || !file_name)
    {
        return;
    }

    if (j->is_array && j->array)
    {
        String s = get_array_as_str(j->array, 1);
        FILE *f = fopen(file_name, "w");
        fwrite(s.str, sizeof(char), s.length, f);
        fclose(f);
        free(s.str);
        return;
    }

    if (!j->dict)
    {
        return;
    }
}

int main(void)
{
    Array *a = init_array(4);
    arr_add_int(a, 666);
    arr_add_double(a, 1234567891.100456);
    arr_add_bool(a, 1);
    arr_add_bool(a, 0);
    JSON *j = init_json(1, a, 0);

    write_json_to_file(j, "out.json");
    destroy_json(j);
    return 0;
}
