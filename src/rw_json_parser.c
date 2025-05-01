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
// destroy_fn should be __asm__("nop") when the value doesn't need to be freed
#define ARR_ADD_OR_RET_DESTROY(add_fn, value, destroy_fn)                      \
    if (!add_fn(a, (value)))                                                   \
    {                                                                          \
        destroy_fn;                                                            \
        return destroy_rw_array_on_error(a);                                   \
    }                                                                          \
    ++nb_elts_parsed

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
    rw_array_t *a  = init_rw_array();
    if (!a || !nb_elts)
    {
        return a;
    }

    size_t nb_elts_parsed = 0;

    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    rw_array_t *tmp_a      = 0;
    rw_dict_t *tmp_d       = 0;

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
            ARR_ADD_OR_RET_DESTROY(rw_array_add_str, s, destroy_string(s));
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
                    ARR_ADD_OR_RET_DESTROY(
                        rw_array_add_double, dwowe.double_value, __asm__("nop")
                    );
                    break;
                case 1:
                    ARR_ADD_OR_RET_DESTROY(
                        rw_array_add_exp_double, dwowe.double_exp_value,
                        __asm__("nop")
                    );
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
                    ARR_ADD_OR_RET_DESTROY(
                        rw_array_add_long, lwowe.long_value, __asm__("nop")
                    );
                    break;
                case 1:
                    ARR_ADD_OR_RET_DESTROY(
                        rw_array_add_exp_long, lwowe.long_exp_value,
                        __asm__("nop")
                    );
                    break;
                case 2:
                    free(sl.str);
                    return destroy_rw_array_on_error(a);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            ARR_ADD_OR_RET_DESTROY(
                rw_array_add_bool, parse_boolean(b, &i) == 4 ? true : false,
                __asm__("nop")
            );
            break;

        case 'n':
            if (!rw_array_add_null(a))
            {
                return destroy_rw_array_on_error(a);
            }
            i += 3;
            ++nb_elts_parsed;
            break;

        case '[':
            if (!(tmp_a = rw_parse_array(b, &i)))
            {
                return destroy_rw_array_on_error(a);
            }
            ARR_ADD_OR_RET_DESTROY(
                rw_array_add_array, tmp_a, destroy_rw_array(tmp_a)
            );
            break;

        case '{':
            if (!(tmp_d = rw_parse_dict(b, &i)))
            {
                return destroy_rw_array_on_error(a);
            }
            ARR_ADD_OR_RET_DESTROY(
                rw_array_add_dict, tmp_d, destroy_rw_dict(tmp_d)
            );
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

// destroy_fn should be __asm__("nop") when the value doesn't need to be freed
#define DICT_ADD_OR_RET_DESTROY(add_fn, value, destroy_fn)                     \
    if (!add_fn(d, key, (value)))                                              \
    {                                                                          \
        destroy_fn;                                                            \
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
    rw_dict_t *d   = init_rw_dict();
    if (!d || !nb_elts)
    {
        return d;
    }

    size_t nb_elts_parsed = 0;

    bool is_waiting_key = true;

    string_t key           = NULL_STRING;
    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    rw_array_t *tmp_a      = 0;
    rw_dict_t *tmp_d       = 0;

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
                DICT_ADD_OR_RET_DESTROY(rw_dict_add_str, s, destroy_string(s));
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
                    DICT_ADD_OR_RET_DESTROY(
                        rw_dict_add_double, dwowe.double_value, __asm__("nop")
                    );
                    break;
                case 1:
                    DICT_ADD_OR_RET_DESTROY(
                        rw_dict_add_exp_double, dwowe.double_exp_value,
                        __asm__("nop")
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
                    DICT_ADD_OR_RET_DESTROY(
                        rw_dict_add_long, lwowe.long_value, __asm__("nop")
                    );
                    break;
                case 1:
                    DICT_ADD_OR_RET_DESTROY(
                        rw_dict_add_exp_long, lwowe.long_exp_value,
                        __asm__("nop")
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
            DICT_ADD_OR_RET_DESTROY(
                rw_dict_add_bool, parse_boolean(b, &i) == 4 ? true : false,
                __asm__("nop")
            );
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
            if (!(tmp_a = rw_parse_array(b, &i)))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            DICT_ADD_OR_RET_DESTROY(
                rw_dict_add_array, tmp_a, destroy_rw_array(tmp_a)
            );
            break;

        case '{':
            if (!(tmp_d = rw_parse_dict(b, &i)))
            {
                return destroy_rw_dict_on_error(d, key);
            }
            DICT_ADD_OR_RET_DESTROY(
                rw_dict_add_dict, tmp_d, destroy_rw_dict(tmp_d)
            );
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
