#include "ro_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <sys/stat.h>

#include "base_json_parser.h"
#include "base_json_storage.h"
#include "ro_json_storage.h"
#include "validator.h"

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
ro_array_t *destroy_ro_array_on_error(ro_array_t *a)
{
    destroy_ro_array(a);
    return 0;
}

ro_dict_t *destroy_ro_dict_on_error(ro_dict_t *d, string_t key)
{
    destroy_string(key);
    destroy_ro_dict(d);
    return 0;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t *ro_parse_array_buff(char *b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at idx + 1 because if we entered this function, it means that we
    // already read a '['
    size_t i = idx == 0 ? 0 : *idx + 1;
    size_t initial_i = i;

    size_t nb_elts = get_nb_elts_array_buff(b, i);

    ro_array_t *a = init_ro_array(nb_elts);
    ro_value_t *values = a->values;
    if (!a || !nb_elts || !values)
    {
        return a;
    }
    size_t insert_idx = 0;

    size_t nb_elts_parsed = 0;
    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        ro_array_t *tmp_a = 0;
        ro_dict_t *tmp_jd = 0;
        switch (c)
        {
        case '"':
            if (!(s = parse_string_buff(b, &i)).str)
            {
                return destroy_ro_array_on_error(a);
            }
            if (insert_idx < nb_elts)
            {
                values[insert_idx++] = RO_VALUE_OF(T_STR, strv, s);
            }
            ++nb_elts_parsed;
            break;

        case '+':
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (!(sl = parse_number_buff(b, &i)).str)
            {
                return destroy_ro_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    if (insert_idx < nb_elts)
                    {
                        values[insert_idx++] =
                            RO_VALUE_OF(T_DOUBLE, doublev, dwowe.double_value);
                    }
                    break;
                case 1:
                    if (insert_idx < nb_elts)
                    {
                        values[insert_idx++] = RO_VALUE_OF(
                            T_EXP_DOUBLE, exp_doublev, dwowe.double_exp_value);
                    }
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_ro_array_on_error(a);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    if (insert_idx < nb_elts)
                    {
                        values[insert_idx++] =
                            RO_VALUE_OF(T_LONG, longv, lwowe.long_value);
                    }
                    break;
                case 1:
                    if (insert_idx < nb_elts)
                    {
                        values[insert_idx++] = RO_VALUE_OF(
                            T_EXP_LONG, exp_longv, lwowe.long_exp_value);
                    }
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_ro_array_on_error(a);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                return destroy_ro_array_on_error(a);
            }
            if (insert_idx < nb_elts)
            {
                values[insert_idx++] =
                    RO_VALUE_OF(T_BOOL, boolv, len == 4 ? true : false);
            }
            ++nb_elts_parsed;
            break;

        case 'n':
            if (insert_idx < nb_elts)
            {
                values[insert_idx++] = (ro_value_t){ .type = T_NULL };
            }
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_a = ro_parse_array_buff(b, &i)))
            {
                return destroy_ro_array_on_error(a);
            }
            if (insert_idx < nb_elts)
            {
                values[insert_idx++] = RO_VALUE_OF(T_ARR, arrayv, tmp_a);
            }
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict_buff(b, &i)))
            {
                return destroy_ro_array_on_error(a);
            }
            if (insert_idx < nb_elts)
            {
                values[insert_idx++] = RO_VALUE_OF(T_DICT, dictv, tmp_jd);
            }
            ++nb_elts_parsed;
            break;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return a;
}

ro_dict_t *ro_parse_dict_buff(char *b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx == 0 ? 0 : *idx + 1;

    size_t nb_elts = get_nb_elts_dict_buff(b, i);

    ro_dict_t *d = init_ro_dict(nb_elts);
    ro_item_t *items = d->items;
    if (!d || !nb_elts || !items)
    {
        return d;
    }
    size_t insert_idx = 0;

    size_t nb_elts_parsed = 0;
    size_t initial_i = i;

    string_t key = NULL_STRING;
    bool is_waiting_key = true;
    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        ro_array_t *tmp_ja = 0;
        ro_dict_t *tmp_jd = 0;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string_buff(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string_buff(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                if (insert_idx < nb_elts && key.str)
                {
                    items[insert_idx++] = RO_ITEM_OF(T_STR, strv, s);
                }
                ++nb_elts_parsed;
            }
            break;

        case '+':
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (!(sl = parse_number_buff(b, &i)).str)
            {
                return destroy_ro_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    if (insert_idx < nb_elts && key.str)
                    {
                        items[insert_idx++] =
                            RO_ITEM_OF(T_DOUBLE, doublev, dwowe.double_value);
                    }
                    break;
                case 1:
                    if (insert_idx < nb_elts && key.str)
                    {
                        items[insert_idx++] = RO_ITEM_OF(
                            T_EXP_DOUBLE, exp_doublev, dwowe.double_exp_value);
                    }
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    if (insert_idx < nb_elts && key.str)
                    {
                        items[insert_idx++] =
                            RO_ITEM_OF(T_LONG, longv, lwowe.long_value);
                    }
                    break;
                case 1:
                    if (insert_idx < nb_elts && key.str)
                    {
                        items[insert_idx++] = RO_ITEM_OF(T_EXP_LONG, exp_longv,
                                                         lwowe.long_exp_value);
                    }
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            if (insert_idx < nb_elts && key.str)
            {
                items[insert_idx++] =
                    RO_ITEM_OF(T_BOOL, boolv, len == 4 ? true : false);
            }
            ++nb_elts_parsed;
            break;

        case 'n':
            if (insert_idx < nb_elts && key.str)
            {
                items[insert_idx++] = (ro_item_t){ .type = T_NULL, .key = key };
            }
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_ja = ro_parse_array_buff(b, &i)))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            if (insert_idx < nb_elts && key.str)
            {
                items[insert_idx++] = RO_ITEM_OF(T_ARR, arrayv, tmp_ja);
            }
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict_buff(b, &i)))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            if (insert_idx < nb_elts && key.str)
            {
                items[insert_idx++] = RO_ITEM_OF(T_DICT, dictv, tmp_jd);
            }
            ++nb_elts_parsed;
            break;

        case ',':
            is_waiting_key = true;
            break;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return d;
}

ro_json_t *ro_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return 0;
    }

    size_t offset = 0;
    if (fseek(f, offset++, SEEK_SET))
    {
        fclose(f);
        return 0;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    size_t nb_chars = st.st_size;

    char c = fgetc(f);
    if (c == '{')
    {
        ro_dict_t *d = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = malloc((nb_chars + 1) * sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET))
            {
                fclose(f);
                free(b);
                return 0;
            }
            b[nb_chars] = 0;
            fread(b, sizeof(char), nb_chars, f);

            if (!is_json_valid_buff(b, nb_chars, true))
            {
                printf("Invalid json file\n");
                free(b);
                fclose(f);
                return 0;
            }

            d = ro_parse_dict_buff(b, 0);
            free(b);
        }
        else
        {
            fclose(f);
            return 0;
        }
        fclose(f);
        return init_ro_json(0, 0, d);
    }
    else if (c == '[')
    {
        ro_array_t *a = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = malloc((nb_chars + 1) * sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET))
            {
                fclose(f);
                free(b);
                return 0;
            }
            b[nb_chars] = 0;
            fread(b, sizeof(char), nb_chars, f);

            if (!is_json_valid_buff(b, nb_chars, false))
            {
                printf("Invalid json file\n");
                free(b);
                fclose(f);
                return 0;
            }

            a = ro_parse_array_buff(b, 0);
            free(b);
        }
        else
        {
            fclose(f);
            return 0;
        }
        fclose(f);
        return init_ro_json(1, a, 0);
    }
    fclose(f);
    return 0;
}
