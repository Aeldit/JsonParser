#include "rw_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "base_json_parser.h"
#include "base_json_storage.h"
#include "validator.h"

/*******************************************************************************
**                           FUNCTIONS DECLARATIONS                           **
*******************************************************************************/
rw_dict_t *rw_parse_dict_buff(char *b, size_t *pos);
rw_dict_t *rw_parse_dict(FILE *f, size_t *pos);

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
rw_array_t *destroy_rw_array_on_error(rw_array_t *a)
{
    destroy_rw_array(a);
    return 0;
}

rw_dict_t *destroy_rw_dict_on_error(rw_dict_t *d, string_t key)
{
    destroy_string(key);
    destroy_rw_dict(d);
    return 0;
}

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
rw_array_t *rw_parse_array_buff(char *b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    size_t i = idx == 0 ? 0 : *idx + 1;

    size_t nb_elts = get_nb_elts_array_buff(b, i);

    rw_array_t *a = calloc(1, sizeof(rw_array_t));
    if (!a || nb_elts == 0)
    {
        return a;
    }

    size_t nb_elts_parsed = 0;
    size_t initial_i = i;

    char c = 0;
    while ((c = b[i]))
    {
        if (nb_elts_parsed >= nb_elts)
        {
            break;
        }

        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        rw_array_t *tmp_a = 0;
        rw_dict_t *tmp_jd = 0;
        switch (c)
        {
        case '"':
            if (!(s = parse_string_buff(b, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_str(a, s);
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
                return destroy_rw_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_array_add_double(a, dwowe.double_value);
                    break;
                case 1:
                    rw_array_add_exp_double(a, dwowe.double_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    rw_array_add_long(a, lwowe.long_value);
                    break;
                case 1:
                    rw_array_add_exp_long(a, lwowe.long_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
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
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_a = rw_parse_array_buff(b, &i)))
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_array(a, tmp_a);
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = rw_parse_dict_buff(b, &i)))
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_dict(a, tmp_jd);
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

rw_dict_t *rw_parse_dict_buff(char *b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx == 0 ? 0 : *idx + 1;

    size_t nb_elts = get_nb_elts_dict_buff(b, i);

    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d || nb_elts == 0)
    {
        return d;
    }

    size_t nb_elts_parsed = 0;
    size_t initial_i = i;

    string_t key = NULL_STRING;
    bool is_waiting_key = true;

    char c = 0;
    while ((c = b[i]))
    {
        if (nb_elts_parsed >= nb_elts)
        {
            break;
        }

        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        rw_array_t *tmp_ja = 0;
        rw_dict_t *tmp_jd = 0;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string_buff(b, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string_buff(b, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                rw_dict_add_str(d, key, s);
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
                return destroy_rw_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_dict_add_double(d, key, dwowe.double_value);
                    break;
                case 1:
                    rw_dict_add_exp_double(d, key, dwowe.double_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    rw_dict_add_long(d, key, lwowe.long_value);
                    break;
                case 1:
                    rw_dict_add_exp_long(d, key, lwowe.long_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
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
                return destroy_rw_dict_on_error(d, key);
            }
            rw_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_ja = rw_parse_array_buff(b, &i)))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            rw_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = rw_parse_dict_buff(b, &i)))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            rw_dict_add_dict(d, key, tmp_jd);
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

rw_array_t *rw_parse_array(FILE *f, size_t *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    size_t i = *pos;

    size_t nb_elts = get_nb_elts_array(f, i);

    rw_array_t *a = calloc(1, sizeof(rw_array_t));
    if (!a || nb_elts == 0)
    {
        ++(*pos);
        return a;
    }

    size_t nb_elts_parsed = 0;

    char c = 0;
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        size_t nb_chars = 0;
        switch (c)
        {
        case '"':
            if (!(s = parse_string(f, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_str(a, s);
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
            if (!(sl = parse_number(f, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_array_add_double(a, dwowe.double_value);
                    break;
                case 1:
                    rw_array_add_exp_double(a, dwowe.double_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    rw_array_add_long(a, lwowe.long_value);
                    break;
                case 1:
                    rw_array_add_exp_long(a, lwowe.long_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            nb_chars = get_nb_chars_in_array(f, i);
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
                if (!(tmp_ja = rw_parse_array_buff(b, 0)))
                {
                    return destroy_rw_array_on_error(a);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_ja = rw_parse_array(f, &i)))
                {
                    return destroy_rw_array_on_error(a);
                }
            }
            rw_array_add_array(a, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            nb_chars = get_nb_chars_in_dict(f, i);
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
                if (!(tmp_jd = rw_parse_dict_buff(b, 0)))
                {
                    return destroy_rw_array_on_error(a);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_jd = rw_parse_dict(f, &i)))
                {
                    return destroy_rw_array_on_error(a);
                }
            }
            rw_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
            break;
        }
    }
    *pos = i - 1;
    return a;
}

rw_dict_t *rw_parse_dict(FILE *f, size_t *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    size_t i = *pos;

    size_t nb_elts = get_nb_elts_dict(f, i);

    rw_dict_t *d = calloc(1, sizeof(rw_dict_t));
    if (!d || nb_elts == 0)
    {
        ++(*pos);
        return d;
    }

    size_t nb_elts_parsed = 0;

    string_t key = NULL_STRING;
    bool is_waiting_key = true;

    char c = 0;
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c != 0)
    {
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len = 0;
        size_t nb_chars = 0;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string(f, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string(f, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                rw_dict_add_str(d, key, s);
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
            if (!(sl = parse_number(f, &i)).str)
            {
                return destroy_rw_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_dict_add_double(d, key, dwowe.double_value);
                    break;
                case 1:
                    rw_dict_add_exp_double(d, key, dwowe.double_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    rw_dict_add_long(d, key, lwowe.long_value);
                    break;
                case 1:
                    rw_dict_add_exp_long(d, key, lwowe.long_exp_value);
                    break;
                case 2:
                default:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean(f, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            rw_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            nb_chars = get_nb_chars_in_array(f, i);
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
                if (!(tmp_ja = rw_parse_array_buff(b, 0)))
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_ja = rw_parse_array(f, &i)))
                {
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            rw_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            nb_chars = get_nb_chars_in_dict(f, i);
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
                if (!(tmp_jd = rw_parse_dict_buff(b, 0)))
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_jd = rw_parse_dict(f, &i)))
                {
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            rw_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
            break;

        case ',':
            is_waiting_key = true;
            break;
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

    size_t offset = 0;
    if (fseek(f, offset++, SEEK_SET) != 0)
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

            if (!is_json_valid(f))
            {
                printf("Invalid json file");
                free(b);
                fclose(f);
                return 0;
            }

            d = rw_parse_dict_buff(b, 0);
            free(b);
        }
        else
        {
            if (!is_json_valid(f))
            {
                printf("Invalid json file");
                fclose(f);
                return 0;
            }
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

            if (!is_json_valid(f))
            {
                printf("Invalid json file");
                fclose(f);
                free(b);
                return 0;
            }

            a = rw_parse_array_buff(b, 0);
            free(b);
        }
        else
        {
            if (!is_json_valid(f))
            {
                printf("Invalid json file");
                fclose(f);
                return 0;
            }
            a = rw_parse_array(f, &offset);
        }
        fclose(f);
        return init_rw_json(1, a, 0);
    }
    fclose(f);
    return 0;
}
