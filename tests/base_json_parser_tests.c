#include <criterion/criterion.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"

#define JSON_TESTS_FILE ("tests/test.json")

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
**                                PARSE_STRING                                **
*******************************************************************************/
void test_parse_string(unsigned long *idx, char *expected_str, char is_buff)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    unsigned long initial_idx = idx ? *idx : 0;
    unsigned long expected_len = expected_str ? strlen(expected_str) : 0;
    string_t s = NULL_STRING;
    char *buff = 0;

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return;
        }
        fread(buff, sizeof(char), nb_chars, f);

        s = parse_string_buff(buff, idx);
    }
    else
    {
        if (idx && fseek(f, (*idx)++, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }
        s = parse_string(f, idx);
    }

    cr_expect(strncmp(s.str, expected_str,
                      s.len < expected_len ? expected_len : s.len)
                  == 0,
              "Expected 'str' to be '%s' but got '%s'", expected_str, s.str);
    cr_expect(s.len == expected_len, "Expected 'len' to be '%lu' but got '%u'",
              expected_len, f && s.len);
    if ((is_buff && buff) && idx)
    {
        cr_expect(*idx - initial_idx - !is_buff == s.len + 1,
                  "Expected '*idx' to be incremented by '%u' but it got "
                  "incremented by '%lu'",
                  s.len + 1, *idx - initial_idx - !is_buff);
    }
    destroy_string(s);
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, parse_string_buff_nullidx)
{
    test_parse_string(0, "", 1);
}

Test(base_json_parser, parse_string_buff_normal_string)
{
    unsigned long idx = 27;
    test_parse_string(&idx, "testing normal string", 1);
}

Test(base_json_parser, parse_string_buff_empty_string)
{
    unsigned long idx = 59;
    test_parse_string(&idx, "", 1);
}

// Files
Test(base_json_parser, parse_string_null_pos)
{
    test_parse_string(0, "", 0);
}

Test(base_json_parser, parse_string_normal_string)
{
    unsigned long idx = 27;
    test_parse_string(&idx, "testing normal string", 0);
}

Test(base_json_parser, parse_string_empty_string)
{
    unsigned long idx = 59;
    test_parse_string(&idx, "", 0);
}

/*******************************************************************************
**                                PARSE_NUMBER                                **
*******************************************************************************/
void test_parse_number(unsigned long *idx,
                       str_and_len_tuple_t expected_str_and_len, char is_buff)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    unsigned long initial_idx = idx ? *idx : 0;
    char *expected_str = expected_str_and_len.str;
    unsigned long expected_len = expected_str_and_len.len;

    str_and_len_tuple_t s = NULL_STR_AND_LEN_TUPLE;
    char *buff = 0;

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return;
        }
        fread(buff, sizeof(char), nb_chars, f);

        s = parse_number_buff(buff, idx);
    }
    else
    {
        if (idx && fseek(f, (*idx)++, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }
        s = parse_number(f, idx);
    }

    cr_expect(strncmp(s.str, expected_str,
                      s.len < expected_len ? expected_len : s.len)
                  == 0,
              "Expected 'str' to be '%s' but got '%s'", expected_str, s.str);
    cr_expect(s.len == expected_len, "Expected 'len' to be '%lu' but got '%u'",
              expected_len, s.len);
    if (f && idx)
    {
        cr_expect(*idx - initial_idx - !is_buff == s.len - 1,
                  "Expected '*idx' to be incremented by '%u' but it got "
                  "incremented by '%lu'",
                  s.len - 1, *idx - initial_idx - !is_buff);
        cr_expect(s.is_float == expected_str_and_len.is_float,
                  "Expected '%s' to %s a float but it %s", s.str,
                  expected_str_and_len.is_float ? "be" : "not be",
                  s.is_float ? "was" : "wasn't");
        cr_expect(s.has_exponent == expected_str_and_len.has_exponent,
                  "Expected '%s' to %s an exponent but it %s", s.str,
                  expected_str_and_len.has_exponent ? "have" : "not have",
                  s.has_exponent ? "did" : "didn't");
    }
    free(s.str);
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, parse_number_buff_positive_nofloat_noexp)
{
    unsigned long pos = 95;
    test_parse_number(&pos, STR_AND_LEN_OF("64220", 5, 0, 0), 1);
}

Test(base_json_parser, parse_number_buff_negative_nofloat_noexp)
{
    unsigned long pos = 110;
    test_parse_number(&pos, STR_AND_LEN_OF("-512", 4, 0, 0), 1);
}

Test(base_json_parser, parse_number_buff_positive_float_noexp)
{
    unsigned long pos = 124;
    test_parse_number(&pos, STR_AND_LEN_OF("642.25", 6, 1, 0), 1);
}

Test(base_json_parser, parse_number_buff_negative_float_noexp)
{
    unsigned long pos = 140;
    test_parse_number(&pos, STR_AND_LEN_OF("-642.25", 7, 1, 0), 1);
}

Test(base_json_parser, parse_number_buff_positive_nofloat_exp)
{
    unsigned long pos = 157;
    test_parse_number(&pos, STR_AND_LEN_OF("2e8", 3, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_nofloat_exp)
{
    unsigned long pos = 170;
    test_parse_number(&pos, STR_AND_LEN_OF("-53e4", 5, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_nofloat_negativeexp)
{
    unsigned long pos = 185;
    test_parse_number(&pos, STR_AND_LEN_OF("200e-10", 7, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_nofloat_negativeexp)
{
    unsigned long pos = 202;
    test_parse_number(&pos, STR_AND_LEN_OF("-251e-10", 8, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_float_exp)
{
    unsigned long pos = 220;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50e10", 9, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_float_exp)
{
    unsigned long pos = 239;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50e10", 10, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_float_negativeexp)
{
    unsigned long pos = 259;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50e-10", 10, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_float_negativeexp)
{
    unsigned long pos = 279;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50e-10", 11, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_nofloat_upperexp)
{
    unsigned long pos = 300;
    test_parse_number(&pos, STR_AND_LEN_OF("2E8", 3, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_nofloat_upperexp)
{
    unsigned long pos = 313;
    test_parse_number(&pos, STR_AND_LEN_OF("-53E4", 5, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_nofloat_uppernegativeexp)
{
    unsigned long pos = 328;
    test_parse_number(&pos, STR_AND_LEN_OF("200E-10", 7, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_nofloat_uppernegativeexp)
{
    unsigned long pos = 345;
    test_parse_number(&pos, STR_AND_LEN_OF("-251E-10", 8, 0, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_float_upperexp)
{
    unsigned long pos = 363;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50E10", 9, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_float_upperexp)
{
    unsigned long pos = 382;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50E10", 10, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_positive_float_uppernegativeexp)
{
    unsigned long pos = 402;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50E-10", 10, 1, 1), 1);
}

Test(base_json_parser, parse_number_buff_negative_float_uppernegativeexp)
{
    unsigned long pos = 422;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50E-10", 11, 1, 1), 1);
}

// Files
Test(base_json_parser, parse_number_positive_nofloat_noexp)
{
    unsigned long pos = 95;
    test_parse_number(&pos, STR_AND_LEN_OF("64220", 5, 0, 0), 0);
}

Test(base_json_parser, parse_number_negative_nofloat_noexp)
{
    unsigned long pos = 110;
    test_parse_number(&pos, STR_AND_LEN_OF("-512", 4, 0, 0), 0);
}

Test(base_json_parser, parse_number_positive_float_noexp)
{
    unsigned long pos = 124;
    test_parse_number(&pos, STR_AND_LEN_OF("642.25", 6, 1, 0), 0);
}

Test(base_json_parser, parse_number_negative_float_noexp)
{
    unsigned long pos = 140;
    test_parse_number(&pos, STR_AND_LEN_OF("-642.25", 7, 1, 0), 0);
}

Test(base_json_parser, parse_number_positive_nofloat_exp)
{
    unsigned long pos = 157;
    test_parse_number(&pos, STR_AND_LEN_OF("2e8", 3, 0, 1), 0);
}

Test(base_json_parser, parse_number_negative_nofloat_exp)
{
    unsigned long pos = 170;
    test_parse_number(&pos, STR_AND_LEN_OF("-53e4", 5, 0, 1), 0);
}

Test(base_json_parser, parse_number_positive_nofloat_negativeexp)
{
    unsigned long pos = 185;
    test_parse_number(&pos, STR_AND_LEN_OF("200e-10", 7, 0, 1), 0);
}

Test(base_json_parser, parse_number_negative_nofloat_negativeexp)
{
    unsigned long pos = 202;
    test_parse_number(&pos, STR_AND_LEN_OF("-251e-10", 8, 0, 1), 0);
}

Test(base_json_parser, parse_number_positive_float_exp)
{
    unsigned long pos = 220;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50e10", 9, 1, 1), 0);
}

Test(base_json_parser, parse_number_negative_float_exp)
{
    unsigned long pos = 239;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50e10", 10, 1, 1), 0);
}

Test(base_json_parser, parse_number_positive_float_negativeexp)
{
    unsigned long pos = 259;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50e-10", 10, 1, 1), 0);
}

Test(base_json_parser, parse_number_negative_float_negativeexp)
{
    unsigned long pos = 279;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50e-10", 11, 1, 1), 0);
}

Test(base_json_parser, parse_number_positive_nofloat_upperexp)
{
    unsigned long pos = 300;
    test_parse_number(&pos, STR_AND_LEN_OF("2E8", 3, 0, 1), 0);
}

Test(base_json_parser, parse_number_negative_nofloat_upperexp)
{
    unsigned long pos = 313;
    test_parse_number(&pos, STR_AND_LEN_OF("-53E4", 5, 0, 1), 0);
}

Test(base_json_parser, parse_number_positive_nofloat_uppernegativeexp)
{
    unsigned long pos = 328;
    test_parse_number(&pos, STR_AND_LEN_OF("200E-10", 7, 0, 1), 0);
}

Test(base_json_parser, parse_number_negative_nofloat_uppernegativeexp)
{
    unsigned long pos = 345;
    test_parse_number(&pos, STR_AND_LEN_OF("-251E-10", 8, 0, 1), 0);
}

Test(base_json_parser, parse_number_positive_float_upperexp)
{
    unsigned long pos = 363;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50E10", 9, 1, 1), 0);
}

Test(base_json_parser, parse_number_negative_float_upperexp)
{
    unsigned long pos = 382;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50E10", 10, 1, 1), 0);
}

Test(base_json_parser, parse_number_positive_float_uppernegativeexp)
{
    unsigned long pos = 402;
    test_parse_number(&pos, STR_AND_LEN_OF("200.50E-10", 10, 1, 1), 0);
}

Test(base_json_parser, parse_number_negative_float_uppernegativeexp)
{
    unsigned long pos = 422;
    test_parse_number(&pos, STR_AND_LEN_OF("-200.50E-10", 11, 1, 1), 0);
}

/*******************************************************************************
**                              PARSE_BOOLEAN_BUFF                            **
*******************************************************************************/
void test_parse_boolean(unsigned long *idx, unsigned long expected_len,
                        char is_buff)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    unsigned long initial_idx = idx ? *idx : 0;
    unsigned long len = 0;
    char *buff = 0;

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return;
        }
        fread(buff, sizeof(char), nb_chars, f);

        len = parse_boolean_buff(buff, idx);
    }
    else
    {
        if (idx && fseek(f, (*idx)++, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        len = parse_boolean(f, idx);
    }

    cr_expect(len == expected_len,
              "Expected the boolean len to be '%lu' but got '%lu'",
              expected_len, len);
    if (buff && idx)
    {
        cr_expect(*idx - initial_idx - !is_buff == len - 1,
                  "Expected '*idx' to be incremented by '%lu' but it got "
                  "incremented by '%lu'",
                  len - 1, *idx - initial_idx - !is_buff);
    }
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, parse_boolean_buff_true)
{
    unsigned long idx = 467;
    test_parse_boolean(&idx, 4, 1);
}

Test(base_json_parser, parse_boolean_buff_false)
{
    unsigned long idx = 481;
    test_parse_boolean(&idx, 5, 1);
}

// Files
Test(base_json_parser, parse_boolean_true)
{
    unsigned long idx = 467;
    test_parse_boolean(&idx, 4, 0);
}

Test(base_json_parser, parse_boolean_false)
{
    unsigned long idx = 481;
    test_parse_boolean(&idx, 5, 0);
}

/*******************************************************************************
**                              GET_NB_ELTS_ARRAY                             **
*******************************************************************************/
void test_get_nb_elts_array(unsigned long idx, unsigned long expected_len,
                            char is_buff)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    unsigned long len = 0;
    char *buff = 0;

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return;
        }
        fread(buff, sizeof(char), nb_chars, f);

        len = get_nb_elts_array_buff(buff, idx);
    }
    else
    {
        if (fseek(f, idx, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }
        len = get_nb_elts_array(f, idx);
    }

    cr_expect(len == expected_len,
              "Expected the length of the array to be '%lu' but got '%lu'",
              expected_len, len);
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, get_nb_elts_array_buff_empty)
{
    test_get_nb_elts_array(519, 0, 1);
}

Test(base_json_parser, get_nb_elts_array_buff_normal)
{
    test_get_nb_elts_array(18, 2, 1);
}

Test(base_json_parser, get_nb_elts_array_buff_nested)
{
    test_get_nb_elts_array(509, 8, 1);
}

// Files
Test(base_json_parser, get_nb_elts_array_empty)
{
    test_get_nb_elts_array(519, 0, 0);
}

Test(base_json_parser, get_nb_elts_array_normal)
{
    test_get_nb_elts_array(18, 2, 0);
}

Test(base_json_parser, get_nb_elts_array_nested)
{
    test_get_nb_elts_array(509, 8, 0);
}

/*******************************************************************************
**                              GET_NB_ELTS_DICT                              **
*******************************************************************************/
void test_get_nb_elts_dict(unsigned long idx, unsigned long expected_len,
                           char is_buff)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    unsigned long len = 0;
    char *buff = 0;

    if (is_buff)
    {
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }

        struct stat st;
        stat(JSON_TESTS_FILE, &st);
        unsigned long nb_chars = st.st_size;

        buff = calloc(nb_chars + 1, sizeof(char));
        if (!buff)
        {
            fclose(f);
            return;
        }
        fread(buff, sizeof(char), nb_chars, f);

        len = get_nb_elts_dict_buff(buff, idx);
    }
    else
    {
        if (fseek(f, idx, SEEK_SET) != 0)
        {
            fclose(f);
            return;
        }
        len = get_nb_elts_dict(f, idx);
    }

    cr_expect(len == expected_len,
              "Expected the length of the dict to be '%lu' but got '%lu'",
              expected_len, len);
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, get_nb_elts_dict_buff_empty)
{
    test_get_nb_elts_dict(813, 0, 1);
}

Test(base_json_parser, get_nb_elts_dict_buff_normal)
{
    test_get_nb_elts_dict(825, 3, 1);
}

Test(base_json_parser, get_nb_elts_dict_buff_nested)
{
    test_get_nb_elts_dict(906, 1, 1);
}

// Files
Test(base_json_parser, get_nb_elts_dict_empty)
{
    test_get_nb_elts_dict(813, 0, 0);
}

Test(base_json_parser, get_nb_elts_dict_normal)
{
    test_get_nb_elts_dict(825, 3, 0);
}

Test(base_json_parser, get_nb_elts_dict_nested)
{
    test_get_nb_elts_dict(906, 1, 0);
}

/*******************************************************************************
**                            GET_NB_CHARS_IN_ARRAY                           **
*******************************************************************************/
void test_get_nb_chars_in_array(unsigned long idx, unsigned long expected_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    if (fseek(f, idx, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }
    unsigned long len = get_nb_chars_in_array(f, idx);

    cr_expect(len == expected_len,
              "Expected the number of characters of the array to be '%lu' but "
              "got '%lu'",
              expected_len, len);
    fclose(f);
}

Test(base_json_parser, get_nb_chars_in_array_empty)
{
    test_get_nb_chars_in_array(519, 0);
}

Test(base_json_parser, get_nb_chars_in_array_normal)
{
    test_get_nb_chars_in_array(18, 49);
}

Test(base_json_parser, get_nb_chars_in_array_nested)
{
    test_get_nb_chars_in_array(509, 277);
}

/*******************************************************************************
**                             GET_NB_CHARS_IN_DICT                           **
*******************************************************************************/
void test_get_nb_chars_in_dict(unsigned long idx, unsigned long expected_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    if (fseek(f, idx, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }
    unsigned long len = get_nb_chars_in_dict(f, idx);

    cr_expect(len == expected_len,
              "Expected the number of characters of the array to be '%lu' but "
              "got '%lu'",
              expected_len, len);
    fclose(f);
}

Test(base_json_parser, get_nb_chars_in_dict_empty)
{
    test_get_nb_chars_in_dict(813, 0);
}

Test(base_json_parser, get_nb_chars_in_dict_normal)
{
    test_get_nb_chars_in_dict(825, 68);
}

Test(base_json_parser, get_nb_chars_in_dict_nested)
{
    test_get_nb_chars_in_dict(905, 211);
}

/*******************************************************************************
**                                   ERRORS                                   **
*******************************************************************************/
Test(base_json_parser, error_str_to_long_nullarg)
{
    long_with_or_without_exponent_t lwowe = str_to_long(0);
    cr_expect(lwowe.has_exponent == 2,
              "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
              "but it was set to '%d'",
              lwowe.has_exponent);
}

Test(base_json_parser, error_str_to_long_nullstr)
{
    str_and_len_tuple_t sl = STR_AND_LEN_OF(0, 1, 0, 0);
    long_with_or_without_exponent_t lwowe = str_to_long(&sl);
    cr_expect(lwowe.has_exponent == 2,
              "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
              "but it was set to '%d'",
              lwowe.has_exponent);
}

Test(base_json_parser, error_str_to_long_zerolen)
{
    str_and_len_tuple_t sl = STR_AND_LEN_OF("", 0, 0, 0);
    long_with_or_without_exponent_t lwowe = str_to_long(&sl);
    cr_expect(lwowe.has_exponent == 2,
              "Expected str_to_long(sl) ; with 'sl' having a null string ; to "
              "set the 'has_exponent' field to 2, but it was set to '%d'",
              lwowe.has_exponent);
}

// str_to_double
Test(base_json_parser, error_str_to_double_nullarg)
{
    double_with_or_without_exponent_t dwowe = str_to_double(0);
    cr_expect(dwowe.has_exponent == 2,
              "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
              "but it was set to '%d'",
              dwowe.has_exponent);
}

Test(base_json_parser, error_str_to_double_nullstr)
{
    str_and_len_tuple_t sl = STR_AND_LEN_OF(0, 1, 0, 0);
    double_with_or_without_exponent_t dwowe = str_to_double(&sl);
    cr_expect(dwowe.has_exponent == 2,
              "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
              "but it was set to '%d'",
              dwowe.has_exponent);
}

Test(base_json_parser, error_str_to_double_zerolen)
{
    str_and_len_tuple_t sl = STR_AND_LEN_OF("", 0, 0, 0);
    double_with_or_without_exponent_t dwowe = str_to_double(&sl);
    cr_expect(dwowe.has_exponent == 2,
              "Expected str_to_long(sl) ; with 'sl' having a null string ; to "
              "set the 'has_exponent' field to 2, but it was set to '%d'",
              dwowe.has_exponent);
}

// is_float
Test(base_json_parser, error_is_float_null_str)
{
    char res = is_float(0, 0);
    cr_expect(!res,
              "Expected is_float(0, len) to return 0, but it returned '%d'",
              res);
}

// has_exponent
Test(base_json_parser, error_has_exponent_null_str)
{
    char res = has_exponent(0, 0);
    cr_expect(!res,
              "Expected has_exponent(0, len) to return 0, but it returned '%d'",
              res);
}

// parse_string_buff
Test(base_json_parser, error_parse_string_buff_nullstr)
{
    unsigned long idx = 5;
    string_t s = parse_string_buff(0, &idx);
    cr_expect(!s.str,
              "Expected parse_string_buff(0, &idx) to a NULL_STRING, but it "
              "returned '%s'",
              s.str);
}

Test(base_json_parser, error_parse_string_buff_nullidx)
{
    string_t s = parse_string_buff("", 0);
    cr_expect(!s.str,
              "Expected parse_string_buff(\"\", 0) to a NULL_STRING, but it "
              "returned '%s'",
              s.str);
}

// parse_string
Test(base_json_parser, error_parse_string_nullstream)
{
    unsigned long idx = 5;
    string_t s = parse_string(0, &idx);
    cr_expect(!s.str,
              "Expected parse_string(0, &idx) to a NULL_STRING, but it "
              "returned { .str = %s, .len = %u }",
              s.str, s.len);
}

Test(base_json_parser, error_parse_string_nullidx)
{
    string_t s = parse_string(stdin, 0);
    cr_expect(!s.str,
              "Expected parse_string(stdin, 0) to a NULL_STRING, but it "
              "returned { .str = %s, .len = %u }",
              s.str, s.len);
}

// parse_number_buff
Test(base_json_parser, error_parse_number_buff_nullstr)
{
    unsigned long idx = 5;
    str_and_len_tuple_t s = parse_number_buff(0, &idx);
    cr_expect(!s.str,
              "Expected parse_number_buff(0, &idx) to a "
              "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %u, "
              ".is_float = %d, .has_exponent = %d }",
              s.str, s.len, s.is_float, s.has_exponent);
}

Test(base_json_parser, error_parse_number_buff_nullidx)
{
    str_and_len_tuple_t s = parse_number_buff("", 0);
    cr_expect(!s.str,
              "Expected parse_number_buff(0, &idx) to a "
              "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %u, "
              ".is_float = %d, .has_exponent = %d }",
              s.str, s.len, s.is_float, s.has_exponent);
}

// parse_number
Test(base_json_parser, error_parse_number_nullstream)
{
    unsigned long idx = 5;
    str_and_len_tuple_t s = parse_number(0, &idx);
    cr_expect(!s.str,
              "Expected parse_number(0, &idx) to a "
              "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %u, "
              ".is_float = %d, .has_exponent = %d }",
              s.str, s.len, s.is_float, s.has_exponent);
}

Test(base_json_parser, error_parse_number_nullidx)
{
    str_and_len_tuple_t s = parse_number(stdin, 0);
    cr_expect(!s.str,
              "Expected parse_number(0, &idx) to a "
              "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %u, "
              ".is_float = %d, .has_exponent = %d }",
              s.str, s.len, s.is_float, s.has_exponent);
}

// parse_boolean_buff
Test(base_json_parser, error_parse_boolean_buff_nullstr)
{
    unsigned long idx = 5;
    unsigned long len = parse_boolean_buff(0, &idx);
    cr_expect(!len,
              "Expected parse_boolean_buff(0, &idx) to return 0, but it "
              "returned '%lu'",
              len);
}

Test(base_json_parser, error_parse_boolean_buff_nullidx)
{
    unsigned long len = parse_boolean_buff("", 0);
    cr_expect(!len,
              "Expected parse_boolean_buff(0, &idx) to return 0, but it "
              "returned '%lu'",
              len);
}

// parse_boolean
Test(base_json_parser, error_parse_boolean_nullstream)
{
    unsigned long idx = 5;
    unsigned long len = parse_boolean(0, &idx);
    cr_expect(!len,
              "Expected parse_boolean(0, &idx) to return 0, but it "
              "returned '%lu'",
              len);
}

Test(base_json_parser, error_parse_boolean_nullidx)
{
    unsigned long len = parse_boolean(stdin, 0);
    cr_expect(!len,
              "Expected parse_boolean(0, &idx) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_elts_array_buff
Test(base_json_parser, error_get_nb_elts_array_buff_nullbuff)
{
    unsigned long len = get_nb_elts_array_buff(0, 5);
    cr_expect(!len,
              "Expected get_nb_elts_array_buff(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_elts_array
Test(base_json_parser, error_get_nb_elts_array_nullstream)
{
    unsigned long len = get_nb_elts_array(0, 5);
    cr_expect(!len,
              "Expected get_nb_elts_array(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_elts_dict_buff
Test(base_json_parser, error_get_nb_elts_dict_buff_nullbuff)
{
    unsigned long len = get_nb_elts_dict_buff(0, 5);
    cr_expect(!len,
              "Expected get_nb_elts_dict_buff(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_elts_dict
Test(base_json_parser, error_get_nb_elts_dict_nullstream)
{
    unsigned long len = get_nb_elts_dict(0, 5);
    cr_expect(!len,
              "Expected get_nb_elts_dict(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_chars_in_array
Test(base_json_parser, error_get_nb_chars_in_array_nullstream)
{
    unsigned long len = get_nb_chars_in_array(0, 5);
    cr_expect(!len,
              "Expected get_nb_chars_in_array(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}

// get_nb_chars_in_dict
Test(base_json_parser, error_get_nb_chars_in_dict_nullstream)
{
    unsigned long len = get_nb_chars_in_dict(0, 5);
    cr_expect(!len,
              "Expected get_nb_chars_in_dict(0, 5) to return 0, but it "
              "returned '%lu'",
              len);
}
