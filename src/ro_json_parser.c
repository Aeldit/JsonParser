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
ro_array_t destroy_ro_array_on_error(ro_array_t a)
{
    destroy_ro_array(a);
    return ERROR_RO_ARRAY;
}

ro_dict_t destroy_ro_dict_on_error(ro_dict_t d, string_t key)
{
    destroy_string(key);
    destroy_ro_dict(d);
    return ERROR_RO_DICT;
}

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
#define ARRAY_ADD(elt)                                                         \
    if (insert_idx < nb_elts)                                                  \
    {                                                                          \
        values[insert_idx++] = (elt);                                          \
    }                                                                          \
    ++nb_elts_parsed

ro_array_t ro_parse_array(char *b, size_t *idx)
{
    if (!b)
    {
        return ERROR_RO_ARRAY;
    }

    // We start at idx + 1 because if we entered this function, it means that we
    // already read a '['
    size_t i         = idx == 0 ? 0 : *idx + 1;
    size_t initial_i = i;

    size_t nb_elts     = get_nb_elts_array(b, i);
    ro_array_t a       = RO_ARRAY(nb_elts);
    ro_value_t *values = a.values;
    if (!nb_elts || !values)
    {
        return a;
    }

    size_t insert_idx     = 0;
    size_t nb_elts_parsed = 0;

    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        string_t s             = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len             = 0;
        ro_array_t tmp_a       = ERROR_RO_ARRAY;
        ro_dict_t tmp_jd       = ERROR_RO_DICT;
        switch (c)
        {
        case '"':
            if (!(s = parse_string(b, &i)).str)
            {
                return destroy_ro_array_on_error(a);
            }
            ARRAY_ADD(ROVAL_STR(s));
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
                return destroy_ro_array_on_error(a);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
                switch (dwowe.has_exponent)
                {
                case 0:
                    ARRAY_ADD(ROVAL_DOUBLE(dwowe.double_value));
                    break;
                case 1:
                    ARRAY_ADD(ROVAL_EXPDOUBLE(dwowe.double_exp_value));
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
                    ARRAY_ADD(ROVAL_LONG(lwowe.long_value));
                    break;
                case 1:
                    ARRAY_ADD(ROVAL_EXPLONG(lwowe.long_exp_value));
                    break;
                case 2:
                    free(sl.str);
                    return destroy_ro_array_on_error(a);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                return destroy_ro_array_on_error(a);
            }
            ARRAY_ADD(ROVAL_BOOL(len == 4));
            break;

        case 'n':
            ARRAY_ADD(ROVAL_NULL);
            i += 3;
            break;

        case '[':
            if (!(tmp_a = ro_parse_array(b, &i)).values)
            {
                return destroy_ro_array_on_error(a);
            }
            ARRAY_ADD(ROVAL_ARR(tmp_a));
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict(b, &i)).items)
            {
                return destroy_ro_array_on_error(a);
            }
            ARRAY_ADD(ROVAL_DICT(tmp_jd));
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

#define DICT_ADD(elt)                                                          \
    if (insert_idx < nb_elts && key.str)                                       \
    {                                                                          \
        items[insert_idx++] = elt;                                             \
    }                                                                          \
    ++nb_elts_parsed;

ro_dict_t ro_parse_dict(char *b, size_t *idx)
{
    if (!b)
    {
        return ERROR_RO_DICT;
    }

    // We start at 1 because if we entered this function, it means that we
    // already read a '{'
    size_t i = idx == 0 ? 0 : *idx + 1;

    size_t nb_elts   = get_nb_elts_dict(b, i);
    ro_dict_t d      = RO_DICT(nb_elts);
    ro_item_t *items = d.items;
    if (!nb_elts || !items)
    {
        return d;
    }

    size_t insert_idx     = 0;
    size_t nb_elts_parsed = 0;
    size_t initial_i      = i;

    string_t key        = NULL_STRING;
    bool is_waiting_key = true;

    char c = 0;
    while ((c = b[i]) && nb_elts_parsed < nb_elts)
    {
        string_t s             = NULL_STRING;
        str_and_len_tuple_t sl = NULL_STR_AND_LEN_TUPLE;
        size_t len             = 0;
        ro_array_t tmp_ja      = ERROR_RO_ARRAY;
        ro_dict_t tmp_jd       = ERROR_RO_DICT;
        switch (c)
        {
        case '"':
            if (is_waiting_key)
            {
                if (!(key = parse_string(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
                }
                is_waiting_key = false;
            }
            else
            {
                if (!(s = parse_string(b, &i)).str)
                {
                    return destroy_ro_dict_on_error(d, key);
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
                return destroy_ro_dict_on_error(d, key);
            }

            if (sl.is_float)
            {
                double_with_or_without_exponent_t dwowe = str_to_double(&sl);
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
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            else
            {
                long_with_or_without_exponent_t lwowe = str_to_long(&sl);
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
                    return destroy_ro_dict_on_error(d, key);
                }
            }
            free(sl.str);
            break;

        case 't':
        case 'f':
            len = parse_boolean(b, &i);
            if ((c == 'f' && len != 5) || (c == 't' && len != 4))
            {
                return destroy_ro_dict_on_error(d, key);
            }
            DICT_ADD(ROIT_BOOL(key, len == 4));
            break;

        case 'n':
            DICT_ADD(ROIT_NULL(key));
            i += 3;
            break;

        case '[':
            if (!(tmp_ja = ro_parse_array(b, &i)).values)
            {
                return destroy_ro_dict_on_error(d, key);
            }
            DICT_ADD(ROIT_ARR(key, tmp_ja));
            break;

        case '{':
            if (!(tmp_jd = ro_parse_dict(b, &i)).items)
            {
                return destroy_ro_dict_on_error(d, key);
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
        *idx += i - initial_i;
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

    size_t offset = 0;
    if (fseek(f, offset++, SEEK_SET))
    {
        fclose(f);
        return ERROR_RO_JSON;
    }

    // Obtains the number of characters in the file
    struct stat st;
    stat(file, &st);
    size_t nb_chars = st.st_size;

    char c = fgetc(f);
    if (c == '{' && nb_chars < MAX_READ_BUFF_SIZE)
    {
        char *b = malloc((nb_chars + 1) * sizeof(char));
        if (!b || fseek(f, offset, SEEK_SET))
        {
            fclose(f);
            free(b);
            return ERROR_RO_JSON;
        }
        b[nb_chars] = 0;
        fread(b, sizeof(char), nb_chars, f);

        if (!is_json_valid(b, nb_chars, true))
        {
            printf("Invalid json file\n");
            free(b);
            fclose(f);
            return ERROR_RO_JSON;
        }

        ro_dict_t d = ro_parse_dict(b, 0);
        free(b);
        fclose(f);
        return RO_JSON(false, ERROR_RO_ARRAY, d);
    }
    else if (c == '[' && nb_chars < MAX_READ_BUFF_SIZE)
    {
        char *b = malloc((nb_chars + 1) * sizeof(char));
        if (!b || fseek(f, offset, SEEK_SET))
        {
            fclose(f);
            free(b);
            return ERROR_RO_JSON;
        }
        b[nb_chars] = 0;
        fread(b, sizeof(char), nb_chars, f);

        if (!is_json_valid(b, nb_chars, false))
        {
            printf("Invalid json file\n");
            free(b);
            fclose(f);
            return ERROR_RO_JSON;
        }

        ro_array_t a = ro_parse_array(b, 0);
        free(b);
        fclose(f);
        return RO_JSON(true, a, ERROR_RO_DICT);
    }
    fclose(f);
    return ERROR_RO_JSON;
}
