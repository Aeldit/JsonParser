#include <criterion/criterion.h>

#include "../src/base_json_parser.h"

str_and_len_tuple_t *get_tuple(char *str, char is_float, char has_exponent)
{
    str_and_len_tuple_t *sl = calloc(1, sizeof(str_and_len_tuple_t));
    if (!sl)
    {
        return 0;
    }
    sl->str = str;
    sl->len = strlen(str);
    sl->is_float = is_float;
    sl->has_exponent = has_exponent;
    return sl;
}

Test(base_json_parser, str_to_long_noexp)
{
    str_and_len_tuple_t *sl = get_tuple("123456", 0, 0);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 0);
    cr_expect(lwowe.long_value == 123456);
    cr_expect(lwowe.long_exp_value.number == 0);
    cr_expect(lwowe.long_exp_value.exponent == 0);
    free(sl);
}

Test(base_json_parser, str_to_long_exp_lower_positive)
{
    str_and_len_tuple_t *sl = get_tuple("123456e10", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == 123456);
    cr_expect(lwowe.long_exp_value.exponent == 10);
    free(sl);
}

Test(base_json_parser, str_to_long_exp_upper_positive)
{
    str_and_len_tuple_t *sl = get_tuple("123456E10", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == 123456);
    cr_expect(lwowe.long_exp_value.exponent == 10);
    free(sl);
}

Test(base_json_parser, str_to_long_exp_lower_negative)
{
    str_and_len_tuple_t *sl = get_tuple("123e-95", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == 123);
    cr_expect(lwowe.long_exp_value.exponent == -95);
    free(sl);
}

Test(base_json_parser, str_to_long_exp_upper_negative)
{
    str_and_len_tuple_t *sl = get_tuple("12E-150", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == 12);
    cr_expect(lwowe.long_exp_value.exponent == -150);
    free(sl);
}

Test(base_json_parser, str_to_long_negative_exp_lower_positive)
{
    str_and_len_tuple_t *sl = get_tuple("-123456e10", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == -123456);
    cr_expect(lwowe.long_exp_value.exponent == 10);
    free(sl);
}

Test(base_json_parser, str_to_long_negative_exp_upper_positive)
{
    str_and_len_tuple_t *sl = get_tuple("-123456E10", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == -123456);
    cr_expect(lwowe.long_exp_value.exponent == 10);
    free(sl);
}

Test(base_json_parser, str_to_long_negative_exp_lower_negative)
{
    str_and_len_tuple_t *sl = get_tuple("-123e-95", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == -123);
    cr_expect(lwowe.long_exp_value.exponent == -95);
    free(sl);
}

Test(base_json_parser, str_to_long_negative_exp_upper_negative)
{
    str_and_len_tuple_t *sl = get_tuple("-12E-150", 0, 1);
    if (!sl)
    {
        return;
    }
    long_with_or_without_exponent_t lwowe = str_to_long(sl);
    cr_expect(lwowe.has_exponent == 1);
    cr_expect(lwowe.long_value == 0);
    cr_expect(lwowe.long_exp_value.number == -12);
    cr_expect(lwowe.long_exp_value.exponent == -150);
    free(sl);
}
