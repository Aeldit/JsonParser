#include "validator.h"

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_parser.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Called after encountering a '+' or '-' sign, or any digit.
**        Starts from the sign or digit that started the number
*/
bool is_number_valid(const char *const buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return false;
    }

    size_t i      = *idx - 1;
    u8 nb_inc_idx = 0;

    while (1)
    {
        // When checking if the current character is not a number, we check if
        // it is > '9' first, because letters comme after it in the ascii table
        // and are more likely to appear in a json file.
        // This way, we can minimize the number of comparisions
        switch (buff[i++])
        {
        case '+':
        case '-':
            // Sign not preceded by an exponent
            if ((nb_inc_idx > 0
                 && (i > 1 && !(buff[i - 2] == 'e' || buff[i - 2] == 'E')))
                // Sign not followed by a digit
                || buff[i] > '9' || buff[i] < '0')
            {
                return false;
            }
            break;

        case 'e':
        case 'E':
            // Exponent not followed by a digit or a sign
            if (buff[i] > '9'
                || (buff[i] < '0' && buff[i] != '+' && buff[i] != '-'))
            {
                return false;
            }
            break;

        case '.':
            // Floating point dot not followed by a digit
            if (buff[i] > '9' || buff[i] < '0')
            {
                return false;
            }
            break;

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
            break;

        default:
            *idx += nb_inc_idx - 1;
            return true;
        }
        ++nb_inc_idx;
    }
    return false;
}

/**
** \brief Checks if the booleans and nulls are spelled correctly, if the numbers
**        are valid, if the quotes are in even number, and brackets and curly
**        brackets have a matching number of opening and closing
*/
bool check_bools_nulls_numbers_counts(
    const char *const buff, size_t buff_len, bool is_dict
)
{
    if (!buff)
    {
        return false;
    }

    size_t nb_quotes                = 0;
    size_t nb_opened_curly_brackets = is_dict ? 1 : 0;
    size_t nb_opened_brackets       = is_dict ? 0 : 1;
    size_t nb_closed_curly_brackets = 0;
    size_t nb_closed_brackets       = 0;

    size_t i = 0;

    bool is_in_string = false;

    while (1)
    {
        if (is_in_string)
        {
            if (buff[i++] == '"' && i > 1 && buff[i - 2] != '\\') // String end
            {
                is_in_string = false;
                ++nb_quotes;
            }
            continue;
        }

        switch (buff[i++])
        {
        case 0:
            break;

        case '"':
            if (!is_in_string)
            {
                is_in_string = true;
                ++nb_quotes;
            }
            continue;

        case 't':
            if (i + 3 < buff_len
                && !(buff[i++] == 'r' && buff[i++] == 'u' && buff[i++] == 'e'))
            {
                return false;
            }
            continue;

        case 'f':
            if (i + 4 < buff_len
                && !(
                    buff[i++] == 'a' && buff[i++] == 'l' && buff[i++] == 's'
                    && buff[i++] == 'e'
                ))
            {
                return false;
            }
            continue;

        case 'n':
            if (i + 3 < buff_len
                && !(buff[i++] == 'u' && buff[i++] == 'l' && buff[i++] == 'l'))
            {
                return false;
            }
            continue;

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
            if (!is_number_valid(buff, &i))
            {
                return false;
            }
            continue;

        case '{':
            ++nb_opened_curly_brackets;
            continue;
        case '[':
            ++nb_opened_brackets;
            continue;
        case '}':
            ++nb_closed_curly_brackets;
            continue;
        case ']':
            ++nb_closed_brackets;
            continue;

        case ':':
        case ',':
        case '\n':
        case '\t':
        case ' ':
            continue;

        default:
            // The character is not part of the json syntax, which means
            // invalid json
            if (i) // To avoid doing '0 - 1' and doing a buffer underflow
            {
                printf(
                    "Found invalid character: '%c' (%d) at index : %zu\n",
                    buff[i - 1], buff[i - 1], i
                );
                // printf("'%s'\n", buff);
            }
            return false;
        }
        break;
    }
    return nb_quotes % 2 == 0
        && nb_opened_curly_brackets == nb_closed_curly_brackets
        && nb_opened_brackets == nb_closed_brackets;
}

bool check_dict_trailing_commas(const char *const buff, size_t *pos);

bool check_array_trailing_commas(const char *const buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i = pos ? *pos : 1;

    char c      = 0;
    char prev_c = 0;
    while (1)
    {
        switch (buff[i++])
        {
        case 0:
            return false;

        case ',':
            prev_c = ',';
            continue;

        case 't':
        case 'n':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case '"':
            GET_STR_LEN(i);
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
            GET_NUM_LEN(i);
            break;

        case ']':
            if (pos)
            {
                *pos = i;
            }
            return prev_c != ',';

        case '[':
            if (!check_array_trailing_commas(buff, &i))
            {
                return false;
            }
            break;

        case '{':
            if (!check_dict_trailing_commas(buff, &i))
            {
                return false;
            }
            break;

        default:
            continue;
        }
        prev_c = 0;
    }
    return true;
}

bool check_dict_trailing_commas(const char *const buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i = pos ? *pos : 1;

    char c      = 0;
    char prev_c = 0;
    while (1)
    {
        switch (buff[i++])
        {
        case 0:
            return false;

        case ',':
            prev_c = ',';
            continue;

        case 't':
        case 'n':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case '"':
            GET_STR_LEN(i);
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
            GET_NUM_LEN(i);
            break;

        case '}':
            if (pos)
            {
                *pos = i;
            }
            return prev_c != ',';

        case '[':
            if (!check_array_trailing_commas(buff, &i))
            {
                return false;
            }
            break;

        case '{':
            if (!check_dict_trailing_commas(buff, &i))
            {
                return false;
            }
            break;

        default:
            continue;
        }
        prev_c = 0;
    }
    return true;
}

bool check_dict_missing_colons_commas(const char *const buff, size_t *pos);

bool check_array_missing_commas(const char *const buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i = pos ? *pos : 0;

    bool value_encountered = false;
    bool is_first_val      = true;
    bool comma_encountered = false;
    bool prev_was_value    = false;

    char c = 0;
    while ((c = buff[i++]))
    {
        if (!value_encountered)
        {
            switch (c)
            {
            case ']':
                if (pos)
                {
                    *pos = i;
                }
                return true;

            case 't':
            case 'f':
            case 'n':
            case '"':
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
            case '[':
            case '{':
                value_encountered = true;
                break;

            default:
                continue;
            }
        }

        switch (c)
        {
        case 't':
        case 'f':
        case 'n':
        case '"':
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
        case '[':
        case '{':
            if (prev_was_value)
            {
                return false;
            }
            prev_was_value = true;
            break;
        }

        switch (c)
        {
        case ',':
            comma_encountered = true;
            prev_was_value    = false;
            continue;

        case 't':
        case 'n':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case '"':
            GET_STR_LEN(i);
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
            GET_NUM_LEN(i);
            break;

        case ']':
            if (pos)
            {
                *pos = i;
            }
            return true;

        case '[':
            if (!check_array_missing_commas(buff, &i))
            {
                return false;
            }
            break;

        case '{':
            if (!check_dict_missing_colons_commas(buff, &i))
            {
                return false;
            }
            break;

        default:
            continue;
        }

        switch (c)
        {
        case 't':
        case 'f':
        case 'n':
        case '"':
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
        case '[':
        case '{':
            if (!comma_encountered && !is_first_val)
            {
                return false;
            }
            comma_encountered = false;
            break;

        default:
            continue;
        }

        if (is_first_val)
        {
            is_first_val = false;
        }
    }
    return false;
}

bool check_dict_missing_colons_commas(const char *const buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i = pos ? *pos : 0;

    size_t nb_colon = 0;
    size_t nb_comma = 0;

    bool colon_encountered = false;
    bool comma_encountered = true;
    bool value_encountered = false;
    bool prev_was_value    = false;

    char c = 0;
    while ((c = buff[i++]))
    {
        if (!value_encountered)
        {
            switch (c)
            {
            case '}':
                if (pos)
                {
                    *pos = i;
                }
                return true;

            case 't':
            case 'f':
            case 'n':
            case '"':
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
            case '[':
            case '{':
                value_encountered = true;
                break;

            default:
                continue;
            }
        }

        switch (c)
        {
        case 't':
        case 'f':
        case 'n':
        case '"':
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
        case '[':
        case '{':
            if (prev_was_value)
            {
                return false;
            }
            prev_was_value = true;
            break;
        }

        switch (c)
        {
        case ':':
            if (!comma_encountered)
            {
                return false;
            }
            ++nb_colon;
            colon_encountered = true;
            comma_encountered = false;
            prev_was_value    = false;
            break;

        case ',':
            if (!colon_encountered)
            {
                return false;
            }
            ++nb_comma;
            comma_encountered = true;
            colon_encountered = false;
            prev_was_value    = false;
            break;

        case 't':
        case 'n':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case '"':
            GET_STR_LEN(i);
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
            GET_NUM_LEN(i);
            break;

        case '}':
            if (pos)
            {
                *pos = i;
            }
            return nb_comma == nb_colon - 1;

        case '[':
            if (!check_array_missing_commas(buff, &i))
            {
                return false;
            }
            break;

        case '{':
            if (!check_dict_missing_colons_commas(buff, &i))
            {
                return false;
            }
            break;
        }
    }
    return false;
}

bool is_json_valid(const char *const buff, size_t buff_len, bool is_dict)
{
#ifdef NOVALIDATION
    return true;
#endif // !NOVALIDATION

    if (!buff)
    {
        return false;
    }

    if (!check_bools_nulls_numbers_counts(buff, buff_len, is_dict))
    {
        return false;
    }

    if (is_dict)
    {
        if (!(check_dict_trailing_commas(buff, 0) && buff[buff_len - 2] == '}'))
        {
            return false;
        }
        if (!check_dict_missing_colons_commas(buff, 0))
        {
            return false;
        }
    }

    if (!(check_array_trailing_commas(buff, 0) && buff[buff_len - 2] == ']'))
    {
        return false;
    }
    if (!check_array_missing_commas(buff, 0))
    {
        return false;
    }
    return true;
}
