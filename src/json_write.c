#include "json_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String get_int_as_str(int value)
{
    int nb_chars = 1;
    int tmp_value = value;
    while (tmp_value /= 10)
    {
        ++nb_chars;
    }

    char *str = calloc(nb_chars + 1, sizeof(char));
    if (!str)
    {
        return (String){ .str = 0, .length = 0 };
    }

    String s = { .str = str, .length = nb_chars };

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
    for (int i = 17; i >= 0; --i)
    {
        if (double_str[i] == '.')
        {
            is_in_decimals = 0;
        }
        if (double_str[i] == '0' && is_in_decimals)
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
        return (String){ .str = 0, .length = 0 };
    }

    String s = { .str = str,
                 .length = nb_chars + nb_decimals - (nb_decimals ? 0 : 1) };
    memcpy(str, double_str, s.length);
    return s;
}

int get_nb_chars_array(Array *a)
{
    if (!a)
    {
        return 0;
    }

    int size = a->size;
    Value *values = a->values;

    uint_fast64_t nb_chars = 3; // 3 : '[' + ']' + '\0'
    for (int i = 0; i < size; ++i)
    {
        Value v = values[i];
        switch (v.type)
        {
        case T_STR:
            nb_chars += v.strv.length;
            break;
        case T_INT:
            nb_chars += get_int_as_str(v.intv);
            break;
        case T_DOUBLE:
            nb_chars += get_double_as_str(v.doublev);
            break;
        case T_BOOL:
            nb_chars += v.boolv ? 4 : 5;
            break;
        }
    }
    return nb_chars;
}

void write_json_to_file(JSON *j, char *file_name)
{
    if (!j)
    {
        return;
    }

    Array *a = j->array;
    if (j->is_array && a)
    {
        int nb_chars = get_nb_chars_array(a);
        char *writtable_json = calloc(nb_chars, sizeof(char));
        if (!writtable_json)
        {
            return;
        }

        writtable_json[0] = '[';
        unsigned insert_idx = 0;

        unsigned size = a->size;
        Value *values = a->values;
        unsigned tmp_nb = 0;
        for (unsigned i = 0; i < size; ++i)
        {
            Value v = values[i];
            switch (v.type)
            {
            case T_STR:
                insert_idx += add_str_to_buff(writtable_json, v.strv.str,
                                              insert_idx, v.strv.length);
                break;
            case T_INT:
                insert_idx += tmp_nb;
                break;
            case T_DOUBLE:
                insert_idx += (v.doublev);
                break;
            case T_BOOL:
                insert_idx += v.boolv ? 4 : 5;
                break;
            }
        }
        writtable_json[nb_chars - 2] = ']';
        printf("%s\n", writtable_json);
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
    arr_add_double(a, 1234567891.123456);
    arr_add_bool(a, 1);
    arr_add_bool(a, 0);
    JSON *j = init_json(1, a, 0);
    // printf("nb chars = %d\n", get_nb_chars_array(a));

    // printf("%d\n", get_nb_chars_int(123456789));
    // printf("%d\n", get_nb_chars_double(1234567891.123456));

    String s = get_int_as_str(5468);
    printf("%s\n", s.str);
    free(s.str);
    String d = get_double_as_str(5468.235);
    printf("%s\n", d.str);
    free(d.str);
    destroy_json(j);
    return 0;
}
