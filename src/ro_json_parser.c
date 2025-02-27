#include "ro_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>
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
ro_array_t *ro_parse_array_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at idx + 1 because if we entered this function, it means that we
    // already read a '['
    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array_buff(b, i);

    ro_array_t *a = init_ro_array(nb_elts);
    if (!a || nb_elts == 0)
    {
        return a;
    }

    char c = 0;
    unsigned long initial_i = i;
    while ((c = b[i]))
    {
        if (nb_elts_parsed >= nb_elts)
        {
            break;
        }

        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        unsigned long len = 0;
        ro_array_t *tmp_a = 0;
        ro_dict_t *tmp_jd = 0;
        switch (c)
        {
        case '"':
            if (!(s = parse_string_buff(b, &i)).str)
            {
                return destroy_ro_array_on_error(a);
            }
            ro_array_add_str(a, s);
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
                    ro_array_add_double(a, dwowe.double_value);
                    break;
                case 1:
                    ro_array_add_exp_double(a, dwowe.double_exp_value);
                    break;
                case 2:
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
                    ro_array_add_long(a, lwowe.long_value);
                    break;
                case 1:
                    ro_array_add_exp_long(a, lwowe.long_exp_value);
                    break;
                case 2:
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
            ro_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            ro_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_a = ro_parse_array_buff(b, &i)))
            {
                return destroy_ro_array_on_error(a);
            }
            ro_array_add_array(a, tmp_a);
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict_buff(b, &i)))
            {
                return destroy_ro_array_on_error(a);
            }
            ro_array_add_dict(a, tmp_jd);
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

ro_dict_t *ro_parse_dict_buff(char *b, unsigned long *idx)
{
    if (!b)
    {
        return 0;
    }

    unsigned long i = idx == 0 ? 0 : *idx + 1;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict_buff(b, i);

    ro_dict_t *d = init_ro_dict(nb_elts);
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
    while ((c = b[i]))
    {
        if (nb_elts_parsed >= nb_elts)
        {
            break;
        }

        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        unsigned long len = 0;
        ro_array_t *tmp_ja = 0;
        ro_dict_t *tmp_jd = 0;
        char test = 0;
        size_t testi = 0;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string_buff(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                is_waiting_key = 0;
            }
            else
            {
                if (!(s = parse_string_buff(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                ro_dict_add_str(d, key, s);
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
                    ro_dict_add_double(d, key, dwowe.double_value);
                    break;
                case 1:
                    ro_dict_add_exp_double(d, key, dwowe.double_exp_value);
                    break;
                case 2:
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
                    ro_dict_add_long(d, key, lwowe.long_value);
                    break;
                case 1:
                    ro_dict_add_exp_long(d, key, lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            test = b[i + 1];
            testi = i;
            len = parse_boolean_buff(b, &i);
            if (IS_NOT_BOOLEAN(c, len))
            {
                // FIX: Memory leak / double free (the quotes count seems to be
                // messed up, the 't' for true is taken from a string
                // "distinct")
                printf("%lu %lu | %c %lu\n", len, i, test, testi);
                i -= len;
                printf("%c%c%c%c%c%c%c%c\n", b[i], b[i + 1], b[i + 2], b[i + 3],
                       b[i + 4], b[i + 5], b[i + 6], b[i + 7]);
                return destroy_ro_dict_on_error(d, key);
            }
            ro_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            ro_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_ja = ro_parse_array_buff(b, &i)))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            ro_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict_buff(b, &i)))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            ro_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
            break;

        case ',':
            is_waiting_key = 1;
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

ro_array_t *ro_parse_array(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_array(f, i);

    ro_array_t *a = init_ro_array(nb_elts);
    if (!a || nb_elts == 0)
    {
        ++(*pos);
        return a;
    }

    char c = 0;
    while (SEEK_AND_GET_CHAR(i) && nb_elts_parsed < nb_elts && c)
    {
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        unsigned long len = 0;
        unsigned long nb_chars = 0;
        switch (c)
        {
        case '"':
            if (!(s = parse_string(f, &i)).str)
            {
                return destroy_ro_array_on_error(a);
            }
            ro_array_add_str(a, s);
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
                return destroy_ro_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    ro_array_add_double(a, dwowe.double_value);
                    break;
                case 1:
                    ro_array_add_exp_double(a, dwowe.double_exp_value);
                    break;
                case 2:
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
                    ro_array_add_long(a, lwowe.long_value);
                    break;
                case 1:
                    ro_array_add_exp_long(a, lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_array_on_error(a);
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
                return destroy_ro_array_on_error(a);
            }
            ro_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            ro_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            nb_chars = get_nb_chars_in_array(f, i);
            ro_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    return destroy_ro_array_on_error(a);
                }
                fread(b, sizeof(char), nb_chars, f);
                if (!(tmp_ja = ro_parse_array_buff(b, 0)))
                {
                    return destroy_ro_array_on_error(a);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_ja = ro_parse_array(f, &i)))
                {
                    return destroy_ro_array_on_error(a);
                }
            }
            ro_array_add_array(a, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            nb_chars = get_nb_chars_in_dict(f, i);
            ro_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    return destroy_ro_array_on_error(a);
                }
                fread(b, sizeof(char), nb_chars, f);
                if (!(tmp_jd = ro_parse_dict_buff(b, 0)))
                {
                    return destroy_ro_array_on_error(a);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_jd = ro_parse_dict(f, &i)))
                {
                    return destroy_ro_array_on_error(a);
                }
            }
            ro_array_add_dict(a, tmp_jd);
            ++nb_elts_parsed;
            break;
        }
    }
    *pos = i - 1;
    return a;
}

ro_dict_t *ro_parse_dict(FILE *f, unsigned long *pos)
{
    if (!f || !pos)
    {
        return 0;
    }

    unsigned long i = *pos;

    unsigned long nb_elts_parsed = 0;
    unsigned long nb_elts = get_nb_elts_dict(f, i);

    ro_dict_t *d = init_ro_dict(nb_elts);
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
        string_t s = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        unsigned long len = 0;
        unsigned long nb_chars = 0;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string(f, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                is_waiting_key = 0;
            }
            else
            {
                if (!(s = parse_string(f, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                ro_dict_add_str(d, key, s);
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
                return destroy_ro_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    ro_dict_add_double(d, key, dwowe.double_value);
                    break;
                case 1:
                    ro_dict_add_exp_double(d, key, dwowe.double_exp_value);
                    break;
                case 2:
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
                    ro_dict_add_long(d, key, lwowe.long_value);
                    break;
                case 1:
                    ro_dict_add_exp_long(d, key, lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_dict_on_error(d, key);
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
                return destroy_ro_dict_on_error(d, key);
            }
            ro_dict_add_bool(d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            ro_dict_add_null(d, key);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            nb_chars = get_nb_chars_in_array(f, i);
            ro_array_t *tmp_ja = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    return destroy_ro_dict_on_error(d, key);
                }
                fread(b, sizeof(char), nb_chars, f);
                if (!(tmp_ja = ro_parse_array_buff(b, 0)))
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_ja = ro_parse_array(f, &i)))
                {
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            ro_dict_add_array(d, key, tmp_ja);
            ++nb_elts_parsed;
            break;

        case '{':
            nb_chars = get_nb_chars_in_dict(f, i);
            ro_dict_t *tmp_jd = 0;
            // If there is enough space, we fill a buffer to read from it
            if (nb_chars < MAX_READ_BUFF_SIZE)
            {
                char *b = calloc(nb_chars + 1, sizeof(char));
                if (!b || fseek(f, i, SEEK_SET) != 0)
                {
                    free(b);
                    return destroy_ro_dict_on_error(d, key);
                }
                fread(b, sizeof(char), nb_chars, f);
                if (!(tmp_jd = ro_parse_dict_buff(b, 0)))
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                free(b);
                i += nb_chars;
            }
            else
            {
                if (!(tmp_jd = ro_parse_dict(f, &i)))
                {
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            ro_dict_add_dict(d, key, tmp_jd);
            ++nb_elts_parsed;
            break;

        case ',':
            is_waiting_key = 1;
            break;
        }
    }
    *pos = i - 1;
    return d;
}

ro_json_t *ro_parse(char *file)
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
        ro_dict_t *d = 0;
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

            if (!is_json_valid_buff(b, nb_chars, 1))
            {
                printf("Invalid json file");
                free(b);
                fclose(f);
                return 0;
            }

            d = ro_parse_dict_buff(b, 0);
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
            d = ro_parse_dict(f, &offset);
        }
        fclose(f);
        return init_ro_json(0, 0, d);
    }
    else if (c == '[')
    {
        ro_array_t *a = 0;
        if (nb_chars < MAX_READ_BUFF_SIZE)
        {
            printf("FILE reading mode");
            char *b = calloc(nb_chars + 1, sizeof(char));
            if (!b || fseek(f, offset, SEEK_SET) != 0)
            {
                fclose(f);
                free(b);
                return 0;
            }
            fread(b, sizeof(char), nb_chars, f);

            if (!is_json_valid_buff(b, nb_chars, 0))
            {
                printf("Invalid json file");
                free(b);
                fclose(f);
                return 0;
            }

            a = ro_parse_array_buff(b, 0);
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
            a = ro_parse_array(f, &offset);
        }
        fclose(f);
        return init_ro_json(1, a, 0);
    }
    fclose(f);
    return 0;
}
