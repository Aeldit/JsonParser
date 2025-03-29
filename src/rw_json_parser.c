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
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
rw_array_t destroy_rw_array_on_error(rw_array_t a)
{
    destroy_rw_array(a);
    return EMPTY_RW_ARRAY;
}

rw_dict_t destroy_rw_dict_on_error(rw_dict_t d, string_t key)
{
    destroy_string(key);
    destroy_rw_dict(d);
    return EMPTY_RW_DICT;
}

/*******************************************************************************
**                              LOCAL FUNCTIONS                               **
*******************************************************************************/
rw_array_t rw_parse_array(const char *b, size_t *idx)
{
    if (!b)
    {
        return EMPTY_RW_ARRAY;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_array(b, i);

    rw_array_t a = EMPTY_RW_ARRAY;
    if (!nb_elts)
    {
        return a;
    }

    size_t nb_elts_parsed = 0;
    size_t initial_i      = i;

    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    size_t len             = 0;

    while (nb_elts_parsed < nb_elts)
    {
        switch (b[i])
        {
        case 0:
            return destroy_rw_array_on_error(a);

        case '"':
            if (!(s = parse_string(b, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_str(&a, s);
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
            if (!(sl = parse_number(b, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_array_add_double(&a, dwowe.double_value);
                    break;
                case 1:
                    rw_array_add_exp_double(&a, dwowe.double_exp_value);
                    break;
                case 2:
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
                    rw_array_add_long(&a, lwowe.long_value);
                    break;
                case 1:
                    rw_array_add_exp_long(&a, lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((b[i] == 'f' && len != 5) || (b[i] == 't' && len != 4))
            {
                return destroy_rw_array_on_error(a);
            }
            rw_array_add_bool(&a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_array_add_null(&a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            rw_array_add_array(&a, rw_parse_array(b, &i));
            ++nb_elts_parsed;
            break;

        case '{':
            rw_array_add_dict(&a, rw_parse_dict(b, &i));
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

rw_dict_t rw_parse_dict(const char *b, size_t *idx)
{
    if (!b)
    {
        return EMPTY_RW_DICT;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_dict(b, i);
    rw_dict_t d    = EMPTY_RW_DICT;
    if (!nb_elts)
    {
        return d;
    }

    size_t nb_elts_parsed = 0;
    size_t initial_i      = i;

    string_t key        = NULL_STRING;
    bool is_waiting_key = true;

    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    size_t len             = 0;

    while (nb_elts_parsed < nb_elts)
    {
        switch (b[i])
        {
        case 0:
            return destroy_rw_dict_on_error(d, key);

        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string(b, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string(b, &i)).str)
                {
                    return destroy_rw_dict_on_error(d, key);
                }
                rw_dict_add_str(&d, key, s);
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
            if (!(sl = parse_number(b, &i)).str)
            {
                return destroy_rw_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_dict_add_double(&d, key, dwowe.double_value);
                    break;
                case 1:
                    rw_dict_add_exp_double(&d, key, dwowe.double_exp_value);
                    break;
                case 2:
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
                    rw_dict_add_long(&d, key, lwowe.long_value);
                    break;
                case 1:
                    rw_dict_add_exp_long(&d, key, lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            free(sl.str);
            ++nb_elts_parsed;
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((b[i] == 'f' && len != 5) || (b[i] == 't' && len != 4))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            rw_dict_add_bool(&d, key, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_dict_add_null(&d, key);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            rw_dict_add_array(&d, key, rw_parse_array(b, &i));
            ++nb_elts_parsed;
            break;

        case '{':
            rw_dict_add_dict(&d, key, rw_parse_dict(b, &i));
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

/*******************************************************************************
**                                 FUNCTIONS **
*******************************************************************************/
rw_json_t rw_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return EMPTY_RW_JSON;
    }

    long offset = 0;
    if (fseek(f, offset++, SEEK_SET))
    {
        fclose(f);
        return EMPTY_RW_JSON;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    size_t nb_chars = st.st_size;

    bool is_array = false;
    rw_array_t a  = EMPTY_RW_ARRAY;
    rw_dict_t d   = EMPTY_RW_DICT;

    int c = fgetc(f);
    if (c == '{' && nb_chars < MAX_READ_BUFF_SIZE)
    {
        char *b = calloc(nb_chars + 1, sizeof(char));
        if (!b || fseek(f, offset, SEEK_SET))
        {
            fclose(f);
            free(b);
            return EMPTY_RW_JSON;
        }
        fread(b, sizeof(char), nb_chars, f);

        if (!is_json_valid(b, nb_chars, true))
        {
            printf("Invalid json file\n");
            free(b);
            fclose(f);
            return EMPTY_RW_JSON;
        }

        d = rw_parse_dict(b, 0);
        free(b);
    }
    else if (c == '[' && nb_chars < MAX_READ_BUFF_SIZE)
    {
        char *b = calloc(nb_chars + 1, sizeof(char));
        if (!b || fseek(f, offset, SEEK_SET))
        {
            fclose(f);
            free(b);
            return EMPTY_RW_JSON;
        }
        fread(b, sizeof(char), nb_chars, f);

        if (!is_json_valid(b, nb_chars, false))
        {
            printf("Invalid json file\n");
            fclose(f);
            free(b);
            return EMPTY_RW_JSON;
        }

        a        = rw_parse_array(b, 0);
        is_array = true;
        free(b);
    }
    fclose(f);
    return RW_JSON(is_array, a, d);
}
