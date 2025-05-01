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
rw_array_t *rw_parse_array(const char *const b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '['
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_array(b, i);
    rw_array_t *a  = calloc(1, sizeof(rw_array_t));
    if (!a || !nb_elts)
    {
        return a;
    }

    size_t nb_elts_parsed = 0;

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
            if (!(sl = parse_number(b, &i)).str)
            {
                return destroy_rw_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    rw_array_add_double(a, dwowe.double_value);
                    break;
                case 1:
                    rw_array_add_exp_double(a, dwowe.double_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    rw_array_add_long(a, lwowe.long_value);
                    break;
                case 1:
                    rw_array_add_exp_long(a, lwowe.long_exp_value);
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
            rw_array_add_bool(a, len == 4 ? 1 : 0);
            ++nb_elts_parsed;
            break;

        case 'n':
            rw_array_add_null(a);
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            rw_array_add_array(a, rw_parse_array(b, &i));
            ++nb_elts_parsed;
            break;

        case '{':
            rw_array_add_dict(a, rw_parse_dict(b, &i));
            ++nb_elts_parsed;
            break;
        }
        ++i;
    }
    if (idx)
    {
        *idx = i;
    }
    return a;
}

#define ADD_OR_RETURN_DESTROY(add_fn, value)                                   \
    if (!add_fn(d, key, (value)))                                              \
    {                                                                          \
        return destroy_rw_dict_on_error(d, key);                               \
    }                                                                          \
    ++nb_elts_parsed

rw_dict_t *rw_parse_dict(const char *const b, size_t *idx)
{
    if (!b)
    {
        return 0;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_dict(b, i);
    rw_dict_t *d   = calloc(1, sizeof(rw_dict_t));
    if (!d || !nb_elts)
    {
        return d;
    }

    size_t nb_elts_parsed = 0;

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
                if (!rw_dict_add_str(d, key, s))
                {
                    destroy_string(s);
                    return destroy_rw_dict_on_error(d, key);
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
            if (!(sl = parse_number(b, &i)).str)
            {
                return destroy_rw_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    ADD_OR_RETURN_DESTROY(
                        rw_dict_add_double, dwowe.double_value
                    );
                    break;
                case 1:
                    ADD_OR_RETURN_DESTROY(
                        rw_dict_add_exp_double, dwowe.double_exp_value
                    );
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    ADD_OR_RETURN_DESTROY(rw_dict_add_long, lwowe.long_value);
                    break;
                case 1:
                    ADD_OR_RETURN_DESTROY(
                        rw_dict_add_exp_long, lwowe.long_exp_value
                    );
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_dict_on_error(d, key);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((b[i] == 'f' && len != 5) || (b[i] == 't' && len != 4))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            ADD_OR_RETURN_DESTROY(rw_dict_add_bool, len == 4 ? true : false);
            break;

        case 'n':
            if (!rw_dict_add_null(d, key))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            ADD_OR_RETURN_DESTROY(rw_dict_add_array, rw_parse_array(b, &i));
            break;

        case '{':
            ADD_OR_RETURN_DESTROY(rw_dict_add_dict, rw_parse_dict(b, &i));
            break;

        case ',':
            is_waiting_key = true;
            break;
        }
        ++i;
    }
    if (idx)
    {
        *idx = i;
    }
    return d;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
rw_json_t *rw_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return 0;
    }

    if (fseek(f, 0, SEEK_SET))
    {
        fclose(f);
        return 0;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    size_t nb_chars = st.st_size - 1;
    if (nb_chars >= MAX_READ_BUFF_SIZE)
    {
        fclose(f);
        return 0;
    }

    bool is_array = false;

    switch (fgetc(f))
    {
    case '{':
        break;
    case '[':
        is_array = true;
        break;
    default:
        fclose(f);
        return 0;
    }

    char *b = malloc((nb_chars + 1) * sizeof(char));
    if (!b || fseek(f, 1, SEEK_SET))
    {
        fclose(f);
        free(b);
        return 0;
    }

    fread(b, sizeof(char), nb_chars, f);
    if (feof(f))
    {
        printf("Unexpected EOF\n");
        free(b);
        fclose(f);
        return 0;
    }
    if (ferror(f))
    {
        printf("Error reading file\n");
        free(b);
        fclose(f);
        return 0;
    }
    b[nb_chars] = 0;

    if (!is_json_valid(b, nb_chars, !is_array))
    {
        printf("Invalid json file\n");
        free(b);
        fclose(f);
        return 0;
    }

    rw_array_t *a = is_array ? rw_parse_array(b, 0) : 0;
    rw_dict_t *d  = is_array ? 0 : rw_parse_dict(b, 0);

    free(b);
    fclose(f);

    rw_json_t *j = calloc(1, sizeof(rw_json_t));
    if (!j)
    {
        return 0;
    }

    j->is_array = is_array;
    if (is_array)
    {
        j->array = a;
    }
    else
    {
        j->dict = d;
    }
    return j;
}
