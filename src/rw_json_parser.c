#include "rw_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <sys/stat.h>

#include "base_json_parser.h"

/*******************************************************************************
**                           FUNCTIONS DECLARATIONS                           **
*******************************************************************************/
rw_dict_t *rw_parse_dict_buff(char *b, unsigned long *pos);
rw_dict_t *rw_parse_dict(FILE *f, unsigned long *pos);

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
rw_array_t *rw_parse_array_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array_buff(b, i);

    rw_array_t *a = calloc(1, sizeof(rw_array_t));
    if (!a || nb_elts == 0)
    {
        return a;
    }

    char c = 0;
    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    unsigned long initial_i = i;
    while (1)
    {
        c = b[i];
        if (c == 0 || nb_elts_parsed >= nb_elts)
        {
            break;
        }

        if (c == '"')
        {
            rw_array_add_str(a, parse_string_buff(b, &i));
            ++nb_elts_parsed;
        }
        else if (IS_NUMBER_START(c))
        {
            str_and_len_tuple_t sl = parse_number_buff(b, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                if (dwowe.has_exponent == 2)
                {
                    continue;
                }
                if (dwowe.has_exponent)
                {
                    rw_array_add_exp_double(a, dwowe.double_exp_value);
                }
                else
                {
                    rw_array_add_double(a, dwowe.double_value);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                if (lwowe.has_exponent == 2)
                {
                    continue;
                }
                if (lwowe.has_exponent)
                {
                    rw_array_add_exp_long(a, lwowe.long_exp_value);
                }
                else
                {
                    rw_array_add_long(a, lwowe.long_value);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            rw_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            rw_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            rw_array_t *tmp_a = rw_parse_array_buff(b, &i);
            if (!tmp_a)
            {
                break;
            }
            rw_array_add_array(a, tmp_a);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            rw_dict_t *tmp_jd = rw_parse_dict_buff(b, &i);
            if (!tmp_jd)
            {
                break;
            }
            rw_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return a;
}

rw_dict_t *rw_parse_dict_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict_buff(b, i);

    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d || nb_elts == 0)
    {
        return d;
    }

    string_t key = NULL_STRING;
    char c = 0;
    char is_waiting_key = 1;
    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    unsigned long initial_i = i;
    while (1)
    {
        c = b[i];
        if (c == 0 || nb_elts_parsed >= nb_elts)
        {
            break;
        }

        if (c == '"')
        {
            if (is_waiting_key)
            {
                key = parse_string_buff(b, &i);
                is_waiting_key = 0;
            }
            else
            {
                rw_dict_add_str(d, key, parse_string_buff(b, &i));
                ++nb_elts_parsed;
            }
        }
        else if (IS_NUMBER_START(c))
        {
            str_and_len_tuple_t sl = parse_number_buff(b, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                if (dwowe.has_exponent == 2)
                {
                    continue;
                }
                if (dwowe.has_exponent)
                {
                    rw_dict_add_exp_double(d, key, dwowe.double_exp_value);
                }
                else
                {
                    rw_dict_add_double(d, key, dwowe.double_value);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                if (lwowe.has_exponent == 2)
                {
                    continue;
                }
                if (lwowe.has_exponent)
                {
                    rw_dict_add_exp_long(d, key, lwowe.long_exp_value);
                }
                else
                {
                    rw_dict_add_long(d, key, lwowe.long_value);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            rw_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            rw_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            rw_array_t *tmp_ja = rw_parse_array_buff(b, &i);
            if (!tmp_ja)
            {
                break;
            }
            rw_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            rw_dict_t *tmp_jd = rw_parse_dict_buff(b, &i);
            if (!tmp_jd)
            {
                break;
            }
            rw_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
        }
        else if (c == ',')
        {
            is_waiting_key = 1;
        }
        ++i;
    }
    if (idx)
    {
        *idx += i - initial_i;
    }
    return d;
}

rw_array_t *rw_parse_array(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array(f, i);

    rw_array_t *a = calloc(1, sizeof(rw_array_t));
    if (!a || nb_elts == 0)
    {
        ++(*pos);
        return a;
    }

    char c = 0;
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        // If we are not in a string or if the string just ended
        if (c == '"')
        {
            rw_array_add_str(a, parse_string(f, &i));
            ++nb_elts_parsed;
        }
        else if (IS_NUMBER_START(c))
        {
            str_and_len_tuple_t sl = parse_number(f, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                if (dwowe.has_exponent == 2)
                {
                    continue;
                }
                if (dwowe.has_exponent)
                {
                    rw_array_add_exp_double(a, dwowe.double_exp_value);
                }
                else
                {
                    rw_array_add_double(a, dwowe.double_value);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                if (lwowe.has_exponent == 2)
                {
                    continue;
                }
                if (lwowe.has_exponent)
                {
                    rw_array_add_exp_long(a, lwowe.long_exp_value);
                }
                else
                {
                    rw_array_add_long(a, lwowe.long_value);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            rw_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            rw_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            unsigned long nb_chars = get_nb_chars_in_array(f, i);

            rw_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_ja = rw_parse_array_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_ja = rw_parse_array(f, &i);
            }

            rw_array_add_array(a, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            unsigned long nb_chars = get_nb_chars_in_dict(f, i);

            rw_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_jd = rw_parse_dict_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_jd = rw_parse_dict(f, &i);
            }

            rw_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
        }
    }
    *pos = i - 1;
    return a;
}

rw_dict_t *rw_parse_dict(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict(f, i);

    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d || nb_elts == 0)
    {
        ++(*pos);
        return d;
    }

    string_t key = NULL_STRING;
    char c = 0;
    char is_waiting_key = 1;
    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        if (c == '"')
        {
            if (is_waiting_key)
            {
                key = parse_string(f, &i);
                is_waiting_key = 0;
            }
            else
            {
                rw_dict_add_str(d, key, parse_string(f, &i));
                ++nb_elts_parsed;
            }
        }
        else if (IS_NUMBER_START(c))
        {
            str_and_len_tuple_t sl = parse_number(f, &i);
            if (!sl.str)
            {
                continue;
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                if (dwowe.has_exponent == 2)
                {
                    continue;
                }
                if (dwowe.has_exponent)
                {
                    rw_dict_add_exp_double(d, key, dwowe.double_exp_value);
                }
                else
                {
                    rw_dict_add_double(d, key, dwowe.double_value);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                if (lwowe.has_exponent == 2)
                {
                    continue;
                }
                if (lwowe.has_exponent)
                {
                    rw_dict_add_exp_long(d, key, lwowe.long_exp_value);
                }
                else
                {
                    rw_dict_add_long(d, key, lwowe.long_value);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
        }
        else if (IS_BOOL_START(c))
        {
            unsigned long len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                continue;
            }
            rw_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
        }
        else if (c == 'n')
        {
            rw_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
        }
        else if (c == '[')
        {
            unsigned long nb_chars = get_nb_chars_in_array(f, i);

            rw_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_ja = rw_parse_array_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_ja = rw_parse_array(f, &i);
            }

            rw_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
        }
        else if (c == '{')
        {
            unsigned long nb_chars = get_nb_chars_in_dict(f, i);

            rw_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    break;
                }
                fread(b, sizeof(char), nb_chars, f);
                tmp_jd = rw_parse_dict_buff(b, 0);
                free(b);
                i += nb_chars;
            }
            else
            {
                tmp_jd = rw_parse_dict(f, &i);
            }

            rw_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
        }
        else if (c == ',')
        {
            is_waiting_key = 1;
        }
    }
    *pos = i - 1;
    return d;
}

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
rw_json_t *rw_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return 0;
    }

    unsigned long offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
    {
        fclose(f);
        return 0;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    unsigned long nb_chars = st.st_size;

    char c = fgetc(f);
    if (c == '{')
    {
        rw_dict_t *d = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = calloc(nb_chars + 1, sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET) != 0)
            {
                fclose(f);
                free(b);
                return 0;
            }
            fread(b, sizeof(char), nb_chars, f);
            d = rw_parse_dict_buff(b, 0);
            free(b);
        }
        else
        {
            d = rw_parse_dict(f, &offset);
        }
        fclose(f);
        return init_rw_json(0, 0, d);
    }
    else if (c == '[')
    {
        rw_array_t *a = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            char *b = calloc(nb_chars + 1, sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET) != 0)
            {
                fclose(f);
                free(b);
                return 0;
            }
            fread(b, sizeof(char), nb_chars, f);
            a = rw_parse_array_buff(b, 0);
            free(b);
        }
        else
        {
            a = rw_parse_array(f, &offset);
        }
        fclose(f);
        return init_rw_json(1, a, 0);
    }
    fclose(f);
    return 0;
}
