#include <criterion/criterion.h>
#include <criterion/logging.h>

#include "../src/base_json_parser.h"

/*******************************************************************************
**                                 STR_TO_LONG                                **
*******************************************************************************/
void test_str_to_long(char *str, char is_float, char has_exponent,
                      char expected_has_exponent, long expected_value,
                      long expected_number, long expected_exponent)
{
    str_and_len_tuple_t *sl = calloc(1, sizeof(str_and_len_tuple_t));
    if (!sl)
    {
        return;
    }
    sl->str = str;
    sl->len = strlen(str);
    sl->is_float = is_float;
    sl->has_exponent = has_exponent;

    long_with_or_without_exponent_t lwowe = str_to_long(sl);

    cr_expect(lwowe.has_exponent == expected_has_exponent,
              "Expected 'has_exponent' to be '%s' but it was '%s'",
              has_exponent ? "true" : "false",
              sl->has_exponent ? "true" : "false");
    cr_expect(lwowe.long_value == expected_value,
              "Expected 'long_value' to be '%ld' but got '%ld'", expected_value,
              lwowe.long_value);
    cr_expect(lwowe.long_exp_value.number == expected_number,
              "Expected 'number' to be '%ld' but got '%ld'", expected_number,
              lwowe.long_exp_value.number);
    cr_expect(lwowe.long_exp_value.exponent == expected_exponent,
              "Expected 'exponent' to be '%ld' but got '%ld'",
              expected_exponent, lwowe.long_exp_value.exponent);
    free(sl);
}

Test(base_json_parser, str_to_long_noexp)
{
    test_str_to_long("123456", 0, 0, 0, 123456, 0, 0);
}

Test(base_json_parser, str_to_long_negative_noexp)
{
    test_str_to_long("-123456", 0, 0, 0, -123456, 0, 0);
}

Test(base_json_parser, str_to_long_exp_lower_positive)
{
    test_str_to_long("123456e10", 0, 1, 1, 0, 123456, 10);
}

Test(base_json_parser, str_to_long_exp_upper_positive)
{
    test_str_to_long("123456E10", 0, 1, 1, 0, 123456, 10);
}

Test(base_json_parser, str_to_long_exp_lower_negative)
{
    test_str_to_long("123e-95", 0, 1, 1, 0, 123, -95);
}

Test(base_json_parser, str_to_long_exp_upper_negative)
{
    test_str_to_long("12E-150", 0, 1, 1, 0, 12, -150);
}

Test(base_json_parser, str_to_long_negative_exp_lower_positive)
{
    test_str_to_long("-123456e10", 0, 1, 1, 0, -123456, 10);
}

Test(base_json_parser, str_to_long_negative_exp_upper_positive)
{
    test_str_to_long("-123456E10", 0, 1, 1, 0, -123456, 10);
}

Test(base_json_parser, str_to_long_negative_exp_lower_negative)
{
    test_str_to_long("-123e-95", 0, 1, 1, 0, -123, -95);
}

Test(base_json_parser, str_to_long_negative_exp_upper_negative)
{
    test_str_to_long("-12E-150", 0, 1, 1, 0, -12, -150);
}

/*******************************************************************************
**                                STR_TO_DOUBLE                               **
*******************************************************************************/
void test_str_to_double(char *str, char is_float, char has_exponent,
                        char expected_has_exponent, double expected_value,
                        double expected_number, long expected_exponent)
{
    str_and_len_tuple_t *sl = calloc(1, sizeof(str_and_len_tuple_t));
    if (!sl)
    {
        return;
    }
    sl->str = str;
    sl->len = strlen(str);
    sl->is_float = is_float;
    sl->has_exponent = has_exponent;

    double_with_or_without_exponent_t dwowe = str_to_double(sl);

    cr_expect(dwowe.has_exponent == expected_has_exponent,
              "Expected 'has_exponent' to be '%s' but it was '%s'",
              has_exponent ? "true" : "false",
              sl->has_exponent ? "true" : "false");
    cr_expect(dwowe.double_value == expected_value,
              "Expected 'double_value' to be '%lf' but got '%lf'",
              expected_value, dwowe.double_value);
    cr_expect(dwowe.double_exp_value.number == expected_number,
              "Expected 'number' to be '%lf' but got '%lf'", expected_number,
              dwowe.double_exp_value.number);
    cr_expect(dwowe.double_exp_value.exponent == expected_exponent,
              "Expected 'exponent' to be '%ld' but got '%ld'",
              expected_exponent, dwowe.double_exp_value.exponent);
    free(sl);
}

Test(base_json_parser, str_to_double_noexp)
{
    test_str_to_double("123.456", 1, 0, 0, 123.456, 0, 0);
}

Test(base_json_parser, str_to_double_negative_noexp)
{
    test_str_to_double("-123.456", 1, 0, 0, -123.456, 0, 0);
}

Test(base_json_parser, str_to_double_exp_lower_positive)
{
    test_str_to_double("12.3e10", 1, 1, 1, 0, 12.3, 10);
}

Test(base_json_parser, str_to_double_exp_upper_positive)
{
    test_str_to_double("1.23456E10", 1, 1, 1, 0, 1.23456, 10);
}

Test(base_json_parser, str_to_double_exp_lower_negative)
{
    test_str_to_double("123e-95", 1, 1, 1, 0, 123, -95);
}

Test(base_json_parser, str_to_double_exp_upper_negative)
{
    test_str_to_double("12.0E-150", 1, 1, 1, 0, 12, -150);
}

Test(base_json_parser, str_to_double_negative_exp_lower_positive)
{
    test_str_to_double("-1234.56e10", 1, 1, 1, 0, -1234.56, 10);
}

Test(base_json_parser, str_to_double_negative_exp_upper_positive)
{
    test_str_to_double("-12.3456E10", 1, 1, 1, 0, -12.3456, 10);
}

Test(base_json_parser, str_to_double_negative_exp_lower_negative)
{
    test_str_to_double("-12.3e-95", 1, 1, 1, 0, -12.3, -95);
}

Test(base_json_parser, str_to_double_negative_exp_upper_negative)
{
    test_str_to_double("-1.2E-150", 1, 1, 1, 0, -1.2, -150);
}

/*******************************************************************************
**                                   IS_FLOAT                                 **
*******************************************************************************/
void test_is_float(char *str, char expected_res)
{
    cr_expect(is_float(str, str ? strlen(str) : 0) == expected_res,
              "Expected '%s' to be a float", str);
}

Test(base_json_parser, is_float_null)
{
    test_is_float(0, 0);
}

Test(base_json_parser, is_float)
{
    test_is_float("12.3456", 1);
}

Test(base_json_parser, is_float_negative)
{
    test_is_float("-12345.6", 1);
}

Test(base_json_parser, is_not_float)
{
    test_is_float("123456", 0);
}

/*******************************************************************************
**                                 HAS_EXPONENT                               **
*******************************************************************************/
void test_has_exponent(char *str, char expected_res)
{
    cr_expect(has_exponent(str, str ? strlen(str) : 0) == expected_res,
              "Expected '%s' to have an exponent", str);
}

Test(base_json_parser, has_exponent_null)
{
    test_has_exponent(0, 0);
}

Test(base_json_parser, has_exponent_long_lower)
{
    test_has_exponent("123456e10", 1);
}

Test(base_json_parser, has_exponent_double_lower)
{
    test_has_exponent("12.3456e14", 1);
}

Test(base_json_parser, has_exponent_long_upper)
{
    test_has_exponent("123456E10", 1);
}

Test(base_json_parser, has_exponent_double_upper)
{
    test_has_exponent("12.3456E14", 1);
}

Test(base_json_parser, has_exponent_negative_long_lower)
{
    test_has_exponent("-123456e50", 1);
}

Test(base_json_parser, has_exponent_negative_double_lower)
{
    test_has_exponent("-12345.6e6", 1);
}

Test(base_json_parser, has_exponent_negative_long_upper)
{
    test_has_exponent("-123456E63", 1);
}

Test(base_json_parser, has_exponent_negative_double_upper)
{
    test_has_exponent("-12345.6E47", 1);
}

Test(base_json_parser, has_exponent_negative_double_upper_negative)
{
    test_has_exponent("-12345.6E-47", 1);
}

Test(base_json_parser, doesnt_have_exponent)
{
    test_has_exponent("123456", 0);
}

/*******************************************************************************
**                              PARSE_STRING_BUFF                             **
*******************************************************************************/
void test_parse_string_buff(char *buff, unsigned long *idx, char *expected_str)
{
    unsigned long initial_idx = idx ? *idx : 0;
    string_t s = parse_string_buff(buff, idx);
    unsigned long expected_len = expected_str ? strlen(expected_str) : 0;

    cr_expect(strncmp(s.str, expected_str,
                      s.len < expected_len ? expected_len : s.len)
                  == 0,
              "Expected 'str' to be '%s' but got '%s'", expected_str, s.str);
    cr_expect(s.len == expected_len, "Expected 'len' to be '%lu' but got '%u'",
              expected_len, s.len);
    if (buff && idx)
    {
        cr_expect(*idx - initial_idx == s.len + 1,
                  "Expected '*idx' to be incremented by '%u' but it got "
                  "incremented by '%lu'",
                  s.len + 1, *idx - initial_idx);
    }
    destroy_string(s);
}

Test(base_json_parser, parse_string_buff_null_buff)
{
    unsigned long idx = 10;
    test_parse_string_buff(0, &idx, 0);
}

Test(base_json_parser, parse_string_buff_null_idx)
{
    test_parse_string_buff("Null index", 0, 0);
}

Test(base_json_parser, parse_string_buff_null_buff_null_idx)
{
    test_parse_string_buff(0, 0, 0);
}

Test(base_json_parser, parse_string_buff_empty_string)
{
    unsigned long idx = 14;
    test_parse_string_buff("{\"test\":false,\"\":\"testing\"}", &idx, "");
}

Test(base_json_parser, parse_string_buff_normal_string)
{
    unsigned long idx = 1;
    test_parse_string_buff("{\"test\":false,\"aaaaah\":\"testing\"}", &idx,
                           "test");
}

Test(base_json_parser, parse_string_buff_long_string)
{
    unsigned long idx = 1;
    test_parse_string_buff("{\"testing with the meaning of the "
                           "meaning, you should find an answer with its own "
                           "meaning\":false,\"aaaaah\":\"testing\"}",
                           &idx,
                           "testing with the meaning of the "
                           "meaning, you should find an answer with its own "
                           "meaning");
}