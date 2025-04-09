#include <criterion/criterion.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../src/base_json_parser.h"

#define JSON_TESTS_FILE ("tests/test.json")

/*******************************************************************************
**                                 STR_TO_LONG                                **
*******************************************************************************/
void test_str_to_long(
    char *str, bool is_float, bool has_exponent, bool expected_has_exponent,
    i64 expected_value, i64 expected_number, i64 expected_exponent
)
{
    str_and_len_tuple_t sl = (str_and_len_tuple_t){
        .str          = str,
        .len          = strlen(str),
        .is_float     = is_float,
        .has_exponent = has_exponent,
    };

    long_with_or_without_exponent_t lwowe = str_to_long(sl);

    cr_expect(
        lwowe.has_exponent == expected_has_exponent,
        "Expected 'has_exponent' to be '%s' but it was '%s'",
        has_exponent ? "true" : "false", sl.has_exponent ? "true" : "false"
    );

    if (lwowe.has_exponent)
    {
        cr_expect(
            lwowe.long_exp_value.number == expected_number,
            "Expected 'number' to be '%ld' but got '%ld'", expected_number,
            lwowe.long_exp_value.number
        );
        cr_expect(
            lwowe.long_exp_value.exponent == expected_exponent,
            "Expected 'exponent' to be '%ld' but got '%ld'", expected_exponent,
            lwowe.long_exp_value.exponent
        );
    }
    else
    {
        cr_expect(
            lwowe.long_value == expected_value,
            "Expected 'long_value' to be '%ld' but got '%ld'", expected_value,
            lwowe.long_value
        );
    }
}

Test(base_json_parser, str_to_long_noexp)
{
    test_str_to_long("123456", false, false, false, 123456, 0, 0);
}

Test(base_json_parser, str_to_long_negative_noexp)
{
    test_str_to_long("-123456", false, false, false, -123456, 0, 0);
}

Test(base_json_parser, str_to_long_exp_lower_positive)
{
    test_str_to_long("123456e10", false, true, true, 0, 123456, 10);
}

Test(base_json_parser, str_to_long_exp_upper_positive)
{
    test_str_to_long("123456E10", false, true, true, 0, 123456, 10);
}

Test(base_json_parser, str_to_long_exp_lower_negative)
{
    test_str_to_long("123e-95", false, true, true, 0, 123, -95);
}

Test(base_json_parser, str_to_long_exp_upper_negative)
{
    test_str_to_long("12E-150", false, true, true, 0, 12, -150);
}

Test(base_json_parser, str_to_long_negative_exp_lower_positive)
{
    test_str_to_long("-123456e10", false, true, true, 0, -123456, 10);
}

Test(base_json_parser, str_to_long_negative_exp_upper_positive)
{
    test_str_to_long("-123456E10", false, true, true, 0, -123456, 10);
}

Test(base_json_parser, str_to_long_negative_exp_lower_negative)
{
    test_str_to_long("-123e-95", false, true, true, 0, -123, -95);
}

Test(base_json_parser, str_to_long_negative_exp_upper_negative)
{
    test_str_to_long("-12E-150", false, true, true, 0, -12, -150);
}

/*******************************************************************************
**                                STR_TO_DOUBLE                               **
*******************************************************************************/
void test_str_to_double(
    char *str, bool is_float, bool has_exponent, bool expected_has_exponent,
    double expected_value, double expected_number, i64 expected_exponent
)
{
    str_and_len_tuple_t sl = (str_and_len_tuple_t){
        .str          = str,
        .len          = strlen(str),
        .is_float     = is_float,
        .has_exponent = has_exponent,
    };

    double_with_or_without_exponent_t dwowe = str_to_double(sl);

    cr_expect(
        dwowe.has_exponent == expected_has_exponent,
        "Expected 'has_exponent' to be '%s' but it was '%s'",
        has_exponent ? "true" : "false", sl.has_exponent ? "true" : "false"
    );

    if (dwowe.has_exponent)
    {
        cr_expect(
            dwowe.double_exp_value.number == expected_number,
            "Expected 'number' to be '%lf' but got '%lf'", expected_number,
            dwowe.double_exp_value.number
        );
        cr_expect(
            dwowe.double_exp_value.exponent == expected_exponent,
            "Expected 'exponent' to be '%ld' but got '%ld'", expected_exponent,
            dwowe.double_exp_value.exponent
        );
    }
    else
    {
        cr_expect(
            dwowe.double_value == expected_value,
            "Expected 'double_value' to be '%lf' but got '%lf'", expected_value,
            dwowe.double_value
        );
    }
}

Test(base_json_parser, str_to_double_noexp)
{
    test_str_to_double("123.456", true, false, false, 123.456, 0, 0);
}

Test(base_json_parser, str_to_double_negative_noexp)
{
    test_str_to_double("-123.456", true, false, false, -123.456, 0, 0);
}

Test(base_json_parser, str_to_double_exp_lower_positive)
{
    test_str_to_double("12.3e10", true, true, true, 0, 12.3, 10);
}

Test(base_json_parser, str_to_double_exp_upper_positive)
{
    test_str_to_double("1.23456E10", true, true, true, 0, 1.23456, 10);
}

Test(base_json_parser, str_to_double_exp_lower_negative)
{
    test_str_to_double("123e-95", true, true, true, 0, 123, -95);
}

Test(base_json_parser, str_to_double_exp_upper_negative)
{
    test_str_to_double("12.0E-150", true, true, true, 0, 12, -150);
}

Test(base_json_parser, str_to_double_negative_exp_lower_positive)
{
    test_str_to_double("-1234.56e10", true, true, true, 0, -1234.56, 10);
}

Test(base_json_parser, str_to_double_negative_exp_upper_positive)
{
    test_str_to_double("-12.3456E10", true, true, true, 0, -12.3456, 10);
}

Test(base_json_parser, str_to_double_negative_exp_lower_negative)
{
    test_str_to_double("-12.3e-95", true, true, true, 0, -12.3, -95);
}

Test(base_json_parser, str_to_double_negative_exp_upper_negative)
{
    test_str_to_double("-1.2E-150", true, true, true, 0, -1.2, -150);
}

/*******************************************************************************
**                                PARSE_STRING                                **
*******************************************************************************/
void test_parse_string(size_t *idx, char *expected_str)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    size_t initial_idx  = idx ? *idx : 0;
    size_t expected_len = expected_str ? strlen(expected_str) : 0;
    string_t s          = NULL_STRING;
    char *buff          = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return;
    }
    fread(buff, sizeof(char), nb_chars, f);

    s = parse_string(buff, idx);

    cr_expect(
        strncmp(
            s.str, expected_str, s.len < expected_len ? expected_len : s.len
        ) == 0,
        "Expected 'str' to be '%s' but got '%s'", expected_str, s.str
    );
    cr_expect(
        s.len == expected_len, "Expected 'len' to be '%zu' but got '%u'",
        expected_len, f && s.len
    );
    if (buff && idx)
    {
        cr_expect(
            *idx - initial_idx == s.len + 1,
            "Expected '*idx' to be incremented by '%zu' but it got "
            "incremented by '%zu'",
            s.len + 1, *idx - initial_idx
        );
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
    test_parse_string(0, "");
}

Test(base_json_parser, parse_string_buff_normal_string)
{
    size_t idx = 27;
    test_parse_string(&idx, "testing normal string");
}

Test(base_json_parser, parse_string_buff_empty_string)
{
    size_t idx = 59;
    test_parse_string(&idx, "");
}

/*******************************************************************************
**                                PARSE_NUMBER                                **
*******************************************************************************/
void test_parse_number(size_t *idx, str_and_len_tuple_t expected_str_and_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    size_t initial_idx  = idx ? *idx : 0;
    char *expected_str  = expected_str_and_len.str;
    size_t expected_len = expected_str_and_len.len;

    str_and_len_tuple_t s = NULL_STR_AND_LEN_TUPLE;
    char *buff            = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return;
    }
    fread(buff, sizeof(char), nb_chars, f);

    s = parse_number(buff, idx);

    cr_expect(
        strncmp(
            s.str, expected_str, s.len < expected_len ? expected_len : s.len
        ) == 0,
        "Expected 'str' to be '%s' but got '%s'", expected_str, s.str
    );
    cr_expect(
        s.len == expected_len, "Expected 'len' to be '%zu' but got '%zu'",
        expected_len, s.len
    );
    if (f && idx)
    {
        cr_expect(
            *idx - initial_idx == s.len - 1,
            "Expected '*idx' to be incremented by '%zu' but it got "
            "incremented by '%zu'",
            s.len - 1, *idx - initial_idx
        );
        cr_expect(
            s.is_float == expected_str_and_len.is_float,
            "Expected '%s' to %s a float but it %s", s.str,
            expected_str_and_len.is_float ? "be" : "not be",
            s.is_float ? "was" : "wasn't"
        );
        cr_expect(
            s.has_exponent == expected_str_and_len.has_exponent,
            "Expected '%s' to %s an exponent but it %s", s.str,
            expected_str_and_len.has_exponent ? "have" : "not have",
            s.has_exponent ? "did" : "didn't"
        );
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
    size_t pos = 95;
    test_parse_number(&pos, STR_AND_LEN("64220", 5));
}

Test(base_json_parser, parse_number_buff_negative_nofloat_noexp)
{
    size_t pos = 110;
    test_parse_number(&pos, STR_AND_LEN("-512", 4));
}

Test(base_json_parser, parse_number_buff_positive_float_noexp)
{
    size_t pos = 124;
    test_parse_number(&pos, STR_AND_LEN_F_OF("642.25", 6));
}

Test(base_json_parser, parse_number_buff_negative_float_noexp)
{
    size_t pos = 140;
    test_parse_number(&pos, STR_AND_LEN_F_OF("-642.25", 7));
}

Test(base_json_parser, parse_number_buff_positive_nofloat_exp)
{
    size_t pos = 157;
    test_parse_number(&pos, STR_AND_LEN_E_OF("2e8", 3));
}

Test(base_json_parser, parse_number_buff_negative_nofloat_exp)
{
    size_t pos = 170;
    test_parse_number(&pos, STR_AND_LEN_E_OF("-53e4", 5));
}

Test(base_json_parser, parse_number_buff_positive_nofloat_negativeexp)
{
    size_t pos = 185;
    test_parse_number(&pos, STR_AND_LEN_E_OF("200e-10", 7));
}

Test(base_json_parser, parse_number_buff_negative_nofloat_negativeexp)
{
    size_t pos = 202;
    test_parse_number(&pos, STR_AND_LEN_E_OF("-251e-10", 8));
}

Test(base_json_parser, parse_number_buff_positive_float_exp)
{
    size_t pos = 220;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("200.50e10", 9));
}

Test(base_json_parser, parse_number_buff_negative_float_exp)
{
    size_t pos = 239;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("-200.50e10", 10));
}

Test(base_json_parser, parse_number_buff_positive_float_negativeexp)
{
    size_t pos = 259;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("200.50e-10", 10));
}

Test(base_json_parser, parse_number_buff_negative_float_negativeexp)
{
    size_t pos = 279;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("-200.50e-10", 11));
}

Test(base_json_parser, parse_number_buff_positive_nofloat_upperexp)
{
    size_t pos = 300;
    test_parse_number(&pos, STR_AND_LEN_E_OF("2E8", 3));
}

Test(base_json_parser, parse_number_buff_negative_nofloat_upperexp)
{
    size_t pos = 313;
    test_parse_number(&pos, STR_AND_LEN_E_OF("-53E4", 5));
}

Test(base_json_parser, parse_number_buff_positive_nofloat_uppernegativeexp)
{
    size_t pos = 328;
    test_parse_number(&pos, STR_AND_LEN_E_OF("200E-10", 7));
}

Test(base_json_parser, parse_number_buff_negative_nofloat_uppernegativeexp)
{
    size_t pos = 345;
    test_parse_number(&pos, STR_AND_LEN_E_OF("-251E-10", 8));
}

Test(base_json_parser, parse_number_buff_positive_float_upperexp)
{
    size_t pos = 363;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("200.50E10", 9));
}

Test(base_json_parser, parse_number_buff_negative_float_upperexp)
{
    size_t pos = 382;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("-200.50E10", 10));
}

Test(base_json_parser, parse_number_buff_positive_float_uppernegativeexp)
{
    size_t pos = 402;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("200.50E-10", 10));
}

Test(base_json_parser, parse_number_buff_negative_float_uppernegativeexp)
{
    size_t pos = 422;
    test_parse_number(&pos, STR_AND_LEN_FE_OF("-200.50E-10", 11));
}

/*******************************************************************************
**                              PARSE_BOOLEAN_BUFF                            **
*******************************************************************************/
void test_parse_boolean(size_t *idx, size_t expected_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    size_t initial_idx = idx ? *idx : 0;
    size_t len         = 0;
    char *buff         = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return;
    }
    fread(buff, sizeof(char), nb_chars, f);

    len = parse_boolean(buff, idx);

    cr_expect(
        len == expected_len,
        "Expected the boolean len to be '%zu' but got '%zu'", expected_len, len
    );
    if (buff && idx)
    {
        cr_expect(
            *idx - initial_idx == len - 1,
            "Expected '*idx' to be incremented by '%zu' but it got "
            "incremented by '%zu'",
            len - 1, *idx - initial_idx
        );
    }
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, parse_boolean_buff_true)
{
    size_t idx = 467;
    test_parse_boolean(&idx, 4);
}

Test(base_json_parser, parse_boolean_buff_false)
{
    size_t idx = 481;
    test_parse_boolean(&idx, 5);
}

/*******************************************************************************
**                              GET_NB_ELTS_ARRAY                             **
*******************************************************************************/
void test_get_nb_elts_array(size_t idx, size_t expected_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    size_t len = 0;
    char *buff = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return;
    }
    fread(buff, sizeof(char), nb_chars, f);

    len = get_nb_elts_array(buff, idx);

    cr_expect(
        len == expected_len,
        "Expected the length of the array to be '%zu' but got '%zu'",
        expected_len, len
    );
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, get_nb_elts_array_buff_empty)
{
    test_get_nb_elts_array(519, 0);
}

Test(base_json_parser, get_nb_elts_array_buff_normal)
{
    test_get_nb_elts_array(18, 2);
}

Test(base_json_parser, get_nb_elts_array_buff_nested)
{
    test_get_nb_elts_array(509, 8);
}

/*******************************************************************************
**                              GET_NB_ELTS_DICT                              **
*******************************************************************************/
void test_get_nb_elts_dict(size_t idx, size_t expected_len)
{
    FILE *f = fopen(JSON_TESTS_FILE, "r");
    if (!f)
    {
        return;
    }

    size_t len = 0;
    char *buff = 0;

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fclose(f);
        return;
    }

    struct stat st;
    stat(JSON_TESTS_FILE, &st);
    size_t nb_chars = st.st_size;

    buff = calloc(nb_chars + 1, sizeof(char));
    if (!buff)
    {
        fclose(f);
        return;
    }
    fread(buff, sizeof(char), nb_chars, f);

    len = get_nb_elts_dict(buff, idx);

    cr_expect(
        len == expected_len,
        "Expected the length of the dict to be '%zu' but got '%zu'",
        expected_len, len
    );
    if (buff)
    {
        free(buff);
    }
    fclose(f);
}

Test(base_json_parser, get_nb_elts_dict_buff_empty)
{
    test_get_nb_elts_dict(813, 0);
}

Test(base_json_parser, get_nb_elts_dict_buff_normal)
{
    test_get_nb_elts_dict(825, 3);
}

Test(base_json_parser, get_nb_elts_dict_buff_nested)
{
    test_get_nb_elts_dict(906, 1);
}

/*******************************************************************************
**                                   ERRORS                                   **
*******************************************************************************/
Test(base_json_parser, error_str_to_long_nullstr)
{
    long_with_or_without_exponent_t lwowe =
        str_to_long(STR_AND_LEN_OF(0, 1, 0, 0));
    cr_expect(
        lwowe.has_exponent == 2,
        "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
        "but it was set to '%d'",
        lwowe.has_exponent
    );
}

Test(base_json_parser, error_str_to_long_zerolen)
{
    long_with_or_without_exponent_t lwowe =
        str_to_long(STR_AND_LEN_OF("", 0, 0, 0));
    cr_expect(
        lwowe.has_exponent == 2,
        "Expected str_to_long(sl) ; with 'sl' having a null string ; to "
        "set the 'has_exponent' field to 2, but it was set to '%d'",
        lwowe.has_exponent
    );
}

// str_to_double
Test(base_json_parser, error_str_to_double_nullstr)
{
    double_with_or_without_exponent_t dwowe =
        str_to_double(STR_AND_LEN_OF(0, 1, 0, 0));
    cr_expect(
        dwowe.has_exponent == 2,
        "Expected str_to_long(0) to set the 'has_exponent' field to 2, "
        "but it was set to '%d'",
        dwowe.has_exponent
    );
}

Test(base_json_parser, error_str_to_double_zerolen)
{
    double_with_or_without_exponent_t dwowe = str_to_double(STR_AND_LEN("", 0));
    cr_expect(
        dwowe.has_exponent == 2,
        "Expected str_to_long(sl) ; with 'sl' having a null string ; to "
        "set the 'has_exponent' field to 2, but it was set to '%d'",
        dwowe.has_exponent
    );
}

// parse_string
Test(base_json_parser, error_parse_string_buff_nullstr)
{
    size_t idx = 5;
    string_t s = parse_string(0, &idx);
    cr_expect(
        !s.str,
        "Expected parse_string_buff(0, &idx) to a NULL_STRING, but it "
        "returned '%s'",
        s.str
    );
}

Test(base_json_parser, error_parse_string_buff_nullidx)
{
    string_t s = parse_string("", 0);
    cr_expect(
        !s.str,
        "Expected parse_string_buff(\"\", 0) to a NULL_STRING, but it "
        "returned '%s'",
        s.str
    );
}

// parse_number
Test(base_json_parser, error_parse_number_buff_nullstr)
{
    size_t idx            = 5;
    str_and_len_tuple_t s = parse_number(0, &idx);
    cr_expect(
        !s.str,
        "Expected parse_number_buff(0, &idx) to a "
        "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %zu, "
        ".is_float = %d, .has_exponent = %d }",
        s.str, s.len, s.is_float, s.has_exponent
    );
}

Test(base_json_parser, error_parse_number_buff_nullidx)
{
    str_and_len_tuple_t s = parse_number("", 0);
    cr_expect(
        !s.str,
        "Expected parse_number_buff(0, &idx) to a "
        "NULL_STR_AND_LEN_TUPLE, but it returned { .str = %s, .len = %zu, "
        ".is_float = %d, .has_exponent = %d }",
        s.str, s.len, s.is_float, s.has_exponent
    );
}

// parse_boolean
Test(base_json_parser, error_parse_boolean_buff_nullstr)
{
    size_t idx = 5;
    size_t len = parse_boolean(0, &idx);
    cr_expect(
        !len,
        "Expected parse_boolean_buff(0, &idx) to return 0, but it "
        "returned '%zu'",
        len
    );
}

Test(base_json_parser, error_parse_boolean_buff_nullidx)
{
    size_t len = parse_boolean("", 0);
    cr_expect(
        !len,
        "Expected parse_boolean_buff(0, &idx) to return 0, but it "
        "returned '%zu'",
        len
    );
}

// get_nb_elts_array
Test(base_json_parser, error_get_nb_elts_array_buff_nullbuff)
{
    size_t len = get_nb_elts_array(0, 5);
    cr_expect(
        !len,
        "Expected get_nb_elts_array_buff(0, 5) to return 0, but it "
        "returned '%zu'",
        len
    );
}

// get_nb_elts_dict
Test(base_json_parser, error_get_nb_elts_dict_buff_nullbuff)
{
    size_t len = get_nb_elts_dict(0, 5);
    cr_expect(
        !len,
        "Expected get_nb_elts_dict_buff(0, 5) to return 0, but it "
        "returned '%zu'",
        len
    );
}
