#include "ro_json_parser.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdlib.h>
#include <sys/stat.h>

#include "base_json_parser.h"
#include "validator.h"

/*******************************************************************************
**                               LOCAL FUNCTIONS                              **
*******************************************************************************/
ro_array_t destroy_ro_array_on_error(ro_array_t *a)
{
    destroy_ro_array(a);
    return ERROR_RO_ARRAY;
}

ro_dict_t destroy_ro_dict_on_error(ro_dict_t *d, string_t *key)
{
    destroy_string(key);
    destroy_ro_dict(d);
    return ERROR_RO_DICT;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t ro_parse_array(const char *const b, size_t *idx)
{
    if (!b)
    {
        return ERROR_RO_ARRAY;
    }

    // We start at idx + 1 because if we entered this function, it means that we
    // already read a '['
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_array(b, i);
    ro_array_t a   = init_ro_array(nb_elts);
    if (!nb_elts || !a.values)
    {
        return a;
    }

    size_t nb_elts_parsed = 0;

    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    size_t len             = 0;
    ro_array_t tmp_a       = ERROR_RO_ARRAY;
    ro_dict_t tmp_jd       = ERROR_RO_DICT;

    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        switch (c)
        {
        case '"':
            if (!(s = parse_string(b, &i)).str)
            {
                return destroy_ro_array_on_error(&a);
            }
            a.values[nb_elts_parsed++] = ROVAL_STR(s);
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
                return destroy_ro_array_on_error(&a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    a.values[nb_elts_parsed++] =
                        ROVAL_DOUBLE(dwowe.double_value);
                    break;
                case 1:
                    a.values[nb_elts_parsed++] =
                        ROVAL_EXPDOUBLE(dwowe.double_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_array_on_error(&a);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    a.values[nb_elts_parsed++] = ROVAL_LONG(lwowe.long_value);
                    break;
                case 1:
                    a.values[nb_elts_parsed++] =
                        ROVAL_EXPLONG(lwowe.long_exp_value);
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_array_on_error(&a);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                return destroy_ro_array_on_error(&a);
            }
            a.values[nb_elts_parsed++] = ROVAL_BOOL(len == 4);
            break;

        case 'n':
            a.values[nb_elts_parsed++] = ROVAL_NULL;
            i += 3;
            break;

        case '[':
            tmp_a = ro_parse_array(b, &i);
            if (tmp_a.size && !tmp_a.values)
            {
                return destroy_ro_array_on_error(&a);
            }
            a.values[nb_elts_parsed++] = ROVAL_ARR(tmp_a);
            break;

        case '{':
            tmp_jd = ro_parse_dict(b, &i);
            if (tmp_jd.size && !tmp_jd.items)
            {
                return destroy_ro_array_on_error(&a);
            }
            a.values[nb_elts_parsed++] = ROVAL_DICT(tmp_jd);
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

#define DICT_ADD(elt)                                                          \
    if (key.str)                                                               \
    {                                                                          \
        d.items[nb_elts_parsed++] = (elt);                                     \
    }

ro_dict_t ro_parse_dict(const char *const b, size_t *idx)
{
    if (!b)
    {
        return ERROR_RO_DICT;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx ? *idx + 1 : 0;

    size_t nb_elts = get_nb_elts_dict(b, i);
    ro_dict_t d    = init_ro_dict(nb_elts);
    if (!nb_elts || !d.items)
    {
        return d;
    }

    size_t nb_elts_parsed = 0;

    string_t key        = NULL_STRING;
    bool is_waiting_key = true;

    string_t s             = NULL_STRING;
    str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
    size_t len             = 0;
    ro_array_t tmp_ja      = ERROR_RO_ARRAY;
    ro_dict_t tmp_jd       = ERROR_RO_DICT;

    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(&d, &key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(&d, &key);
                }
                DICT_ADD(ROIT_STR(key, s));
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
                return destroy_ro_dict_on_error(&d, &key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    DICT_ADD(ROIT_DOUBLE(key, dwowe.double_value));
                    break;
                case 1:
                    DICT_ADD(ROIT_EXPDOUBLE(key, dwowe.double_exp_value));
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_dict_on_error(&d, &key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(sl);
                switch (lwowe.has_exponent)
                {
                case 0:
                    DICT_ADD(ROIT_LONG(key, lwowe.long_value));
                    break;
                case 1:
                    DICT_ADD(ROIT_EXPLONG(key, lwowe.long_exp_value));
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_dict_on_error(&d, &key);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                return destroy_ro_dict_on_error(&d, &key);
            }
            DICT_ADD(ROIT_BOOL(key, len == 4));
            break;

        case 'n':
            DICT_ADD(ROIT_NULL(key));
            i += 3;
            break;

        case '[':
            tmp_ja = ro_parse_array(b, &i);
            if (tmp_ja.size && !tmp_ja.values)
            {
                return destroy_ro_dict_on_error(&d, &key);
            }
            DICT_ADD(ROIT_ARR(key, tmp_ja));
            break;

        case '{':
            tmp_jd = ro_parse_dict(b, &i);
            if (tmp_jd.size && !tmp_jd.items)
            {
                return destroy_ro_dict_on_error(&d, &key);
            }
            DICT_ADD(ROIT_DICT(key, tmp_jd));
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

ro_json_t ro_parse(char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
    {
        return ERROR_RO_JSON;
    }

    if (fseek(f, 0, SEEK_SET))
    {
        fclose(f);
        return ERROR_RO_JSON;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    // - 1 because the first char is not put in the buffer
    size_t nb_chars = st.st_size - 1;
    if (nb_chars >= MAX_READ_BUFF_SIZE)
    {
        return ERROR_RO_JSON;
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
        return ERROR_RO_JSON;
    }

    char *b = malloc((nb_chars + 1) * sizeof(char));
    if (!b || fseek(f, 1, SEEK_SET))
    {
        fclose(f);
        free(b);
        return ERROR_RO_JSON;
    }

    fread(b, sizeof(char), nb_chars, f);
    if (feof(f))
    {
        printf("Unexpected EOF\n");
        free(b);
        fclose(f);
        return ERROR_RO_JSON;
    }
    if (ferror(f))
    {
        printf("Error reading file\n");
        free(b);
        fclose(f);
        return ERROR_RO_JSON;
    }

    b[nb_chars] = 0;

    if (!is_json_valid(b, nb_chars, !is_array))
    {
        printf("Invalid json file\n");
        free(b);
        fclose(f);
        return ERROR_RO_JSON;
    }

    ro_array_t a = is_array ? ro_parse_array(b, 0) : ERROR_RO_ARRAY;
    ro_dict_t d  = is_array ? ERROR_RO_DICT : ro_parse_dict(b, 0);

    free(b);
    fclose(f);
    return init_ro_json(is_array, a, d);
}
