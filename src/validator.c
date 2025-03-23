#include "validator.h"

#include "base_json_parser.h"

/**
** \brief Called after encountering a '+' or '-' sign, or any digit.
**        Starts from the sign or digit that started the number
*/
bool is_number_valid(char *buff, size_t *idx)
{
    if (!buff || !idx)
    {
        return false;
    }

    size_t i      = *idx - 1;
    u8 nb_inc_idx = 0;

    char c      = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        switch (c)
        {
        case '+':
        case '-':
            // Sign not preceded by an exponent
            if ((nb_inc_idx > 0 && !(prev_c == 'e' || prev_c == 'E'))
                // Sign not followed by a digit
                || !(buff[i] >= '0' && buff[i] <= '9'))
            {
                return false;
            }
            break;

        case 'e':
        case 'E':
            // Exponent not followed by a digit or a sign
            if (!((buff[i] >= '0' && buff[i] <= '9') || buff[i] == '+'
                  || buff[i] == '-'))
            {
                return false;
            }
            break;

        case '.':
            // Floating point dot not followed by a digit
            if (!(buff[i] >= '0' && buff[i] <= '9'))
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
        prev_c = c;
    }
    return false;
}

/**
** \brief Checks if the booleans and nulls are spelled correctly, if the numbers
**        are valid, if the quotes are in even number, and brackets and curly
**        brackets have a matching number of opening and closing
*/
bool check_bools_nulls_numbers_counts(char *buff, size_t buff_len, bool is_dict)
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

    char c      = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        if (is_in_string)
        {
            if (c == '"' && prev_c != '\\') // String end
            {
                is_in_string = false;
                ++nb_quotes;
                prev_c = c;
            }
            continue;
        }

        switch (c)
        {
        case '"':
            if (!is_in_string)
            {
                is_in_string = true;
                ++nb_quotes;
            }
            break;

        case 't':
            if (i + 3 < buff_len)
            {
                if (!(buff[i++] == 'r' && buff[i++] == 'u' && buff[i++] == 'e'))
                {
                    return false;
                }
            }
            break;

        case 'f':
            if (i + 4 < buff_len)
            {
                if (!(buff[i++] == 'a' && buff[i++] == 'l' && buff[i++] == 's'
                      && buff[i++] == 'e'))
                {
                    return false;
                }
            }
            break;

        case 'n':
            if (i + 3 < buff_len)
            {
                if (!(buff[i++] == 'u' && buff[i++] == 'l' && buff[i++] == 'l'))
                {
                    return false;
                }
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
            if (!is_number_valid(buff, &i))
            {
                return false;
            }
            break;

        case '{':
            ++nb_opened_curly_brackets;
            break;
        case '[':
            ++nb_opened_brackets;
            break;
        case '}':
            ++nb_closed_curly_brackets;
            break;
        case ']':
            ++nb_closed_brackets;
            break;

        case ':':
        case ',':
        case '\n':
        case '\t':
        case ' ':
            break;

        default:
            // The character is not part of the json syntax, which means
            // invalid json
            printf(
                "Found invalid character: '%c' (%d) at index : %zu\n", c, c, i
            );
            return false;
        }
        prev_c = c;
    }
    return nb_quotes % 2 == 0
        && nb_opened_curly_brackets == nb_closed_curly_brackets
        && nb_opened_brackets == nb_closed_brackets;
}

bool check_dict_trailing_commas(char *buff, size_t *pos);

bool check_array_trailing_commas(char *buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i         = pos ? *pos : 1;
    size_t initial_i = i;

    char c      = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        switch (c)
        {
        case ',':
            prev_c = ',';
            continue;

        case 't':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case 'n':
            i += 3;
            break;

        case '"':
            i += get_str_len(buff, i);
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
            i += get_num_len(buff, i);
            break;

        case ']':
            if (pos)
            {
                *pos += i - initial_i;
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

bool check_dict_trailing_commas(char *buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i         = pos ? *pos : 1;
    size_t initial_i = i;

    char c      = 0;
    char prev_c = 0;
    while ((c = buff[i++]))
    {
        switch (c)
        {
        case ',':
            prev_c = ',';
            continue;

        case 't':
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case 'n':
            i += 3;
            break;

        case '"':
            i += get_str_len(buff, i);
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
            i += get_num_len(buff, i);
            break;

        case '}':
            if (pos)
            {
                *pos += i - initial_i;
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

bool check_dict_missing_colons_commas(char *buff, size_t *pos);

bool check_array_missing_commas(char *buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i         = pos ? *pos : 0;
    size_t initial_i = i;

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
                    *pos += i - initial_i;
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
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case 'n':
            i += 3;
            break;

        case '"':
            i += get_str_len(buff, i);
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
            i += get_num_len(buff, i);
            break;

        case ']':
            if (pos)
            {
                *pos += i - initial_i;
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

bool check_dict_missing_colons_commas(char *buff, size_t *pos)
{
    if (!buff)
    {
        return false;
    }

    size_t i         = pos ? *pos : 0;
    size_t initial_i = i;

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
                    *pos += i - initial_i;
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
            i += 3;
            break;

        case 'f':
            i += 4;
            break;

        case 'n':
            i += 3;
            break;

        case '"':
            i += get_str_len(buff, i);
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
            i += get_num_len(buff, i);
            break;

        case '}':
            if (pos)
            {
                *pos += i - initial_i;
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

bool is_json_valid(char *buff, size_t buff_len, bool is_dict)
{
#ifdef SEPVALIDATION
    return true;
#endif // !SEPVALIDATION

    if (!buff)
    {
        return false;
    }
    return check_bools_nulls_numbers_counts(buff, buff_len, is_dict) && is_dict
        ? (check_dict_trailing_commas(buff, 0) && buff[buff_len - 3] == '}'
           && check_dict_missing_colons_commas(buff, 0))
        : (check_array_trailing_commas(buff, 0) && buff[buff_len - 3] == ']'
           && check_array_missing_commas(buff, 0));
}
