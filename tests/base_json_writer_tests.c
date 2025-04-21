#include <criterion/criterion.h>

#include "../src/base_json_writer.h"

/*******************************************************************************
**                              GET_NB_CHAR_LONG                              **
*******************************************************************************/
Test(base_json_writer, get_nb_char_long_zero)
{
    u8 nb_char = get_nb_char_long(0);
    cr_expect(
        nb_char == 1,
        "Expected get_nb_char_long(0) to return 1, but it returned '%u'",
        nb_char
    );
}

Test(base_json_writer, get_nb_char_long_positive)
{
    u8 nb_char = get_nb_char_long(123456);
    cr_expect(
        nb_char == 6,
        "Expected get_nb_char_long(123456) to return 6, but it returned '%u'",
        nb_char
    );
}

Test(base_json_writer, get_nb_char_long_negative)
{
    u8 nb_char = get_nb_char_long(-123456);
    cr_expect(
        nb_char == 7,
        "Expected get_nb_char_long(-123456) to return 7, but it returned '%u'",
        nb_char
    );
}

/*******************************************************************************
**                                  ADD_LINK                                  **
*******************************************************************************/
Test(base_json_writer, add_link_normal)
{
    string_linked_list_t sll = (string_linked_list_t){ 0 };
    string_link_t *sl1       = calloc(1, sizeof(string_link_t));
    string_link_t *sl2       = calloc(1, sizeof(string_link_t));
    string_link_t *sl3       = calloc(1, sizeof(string_link_t));
    string_link_t *sl4       = calloc(1, sizeof(string_link_t));
    if (!sl1 || !sl2 || !sl3 || !sl4)
    {
        free(sl1);
        free(sl2);
        free(sl3);
        free(sl4);
        return;
    }
    sll.head = sl1;
    sll.tail = sl4;

    sl1->next = sl2;
    sl2->next = sl3;
    sl3->next = sl4;

    add_link(&sll, STRING_OF("test", 4), true);
    size_t len         = 0;
    string_link_t *tmp = sll.head;
    while (tmp)
    {
        ++len;
        tmp = tmp->next;
    }
    cr_expect(
        len == 5,
        "Expected the length of a string_linked_list_t with 4 elements to be 5 "
        "after calling the add_link() function once, but got '%zu'",
        len
    );
    destroy_linked_list(&sll);
}

/*******************************************************************************
**                               GET_LONG_AS_STR                              **
*******************************************************************************/
void test_get_long_as_str(i64 n, size_t len, char *nstr)
{
    string_t s = get_long_as_str(n);
    cr_expect(
        strings_equals(s, STRING_OF(nstr, len)),
        "Expected get_long_as_str(%zu) to return the string { .str = "
        "\"%zu\", .len = %zu }, but got { .str = \"%s\", .len = %zu }",
        n, n, len, s.str, s.len
    );
    destroy_string(s);
}

Test(base_json_writer, get_long_as_str_zero)
{
    test_get_long_as_str(0, 1, "0");
}

Test(base_json_writer, get_long_as_str_positive)
{
    test_get_long_as_str(123456789, 9, "123456789");
}

Test(base_json_writer, get_long_as_str_negative)
{
    test_get_long_as_str(-123456789, 10, "-123456789");
}

/*******************************************************************************
**                              GET_DOUBLE_AS_STR                             **
*******************************************************************************/
void test_get_double_as_str(double n, size_t len, char *nstr)
{
    string_t s = get_double_as_str(n);
    cr_expect(
        strings_equals(s, STRING_OF(nstr, len)),
        "Expected get_double_as_str(%s) to return the string { .str = "
        "\"%s\", .len = %zu }, but got { .str = \"%s\", .len = %zu }",
        nstr, nstr, len, s.str, s.len
    );
    destroy_string(s);
}

Test(base_json_writer, get_double_as_str_zero_zero)
{
    test_get_double_as_str(0.0, 8, "0.000000");
}

Test(base_json_writer, get_double_as_str_positive)
{
    test_get_double_as_str(1234.56789, 11, "1234.567890");
}

Test(base_json_writer, get_double_as_str_negative)
{
    test_get_double_as_str(-123456.789, 14, "-123456.789000");
}

/*******************************************************************************
**                             GET_LONG_EXP_AS_STR                            **
*******************************************************************************/
void test_get_long_exp_as_str(i64 n, i64 e, size_t len, char *nstr)
{
    exp_long_t el = (exp_long_t){ .number = n, .exponent = e };
    string_t s    = get_exp_long_as_str(el);
    cr_expect(
        strings_equals(s, STRING_OF(nstr, len)),
        "Expected get_exp_long_as_str(%lde%ld) to return the string { .str = "
        "\"%lde%ld\", .len = %zu }, but got { .str = \"%s\", .len = %zu }",
        el.number, el.exponent, el.number, el.exponent, len, s.str, s.len
    );
    destroy_string(s);
}

Test(base_json_writer, get_long_exp_as_str_small)
{
    test_get_long_exp_as_str(2, 3, 3, "2e3");
}

Test(base_json_writer, get_long_exp_as_str_positive)
{
    test_get_long_exp_as_str(123456789, 2, 11, "123456789e2");
}

Test(base_json_writer, get_long_exp_as_str_negative)
{
    test_get_long_exp_as_str(-123456789, 5, 12, "-123456789e5");
}

/*******************************************************************************
**                            GET_DOUBLE_EXP_AS_STR                           **
*******************************************************************************/
void test_get_double_exp_as_str(double n, i64 e, size_t len, char *nstr)
{
    exp_double_t ed = (exp_double_t){ .number = n, .exponent = e };
    string_t s      = get_exp_double_as_str(ed);
    cr_expect(
        strings_equals(s, STRING_OF(nstr, len)),
        "Expected get_double_exp_as_str(%lfe%ld) to return the string { .str = "
        "\"%lfe%ld\", .len = %zu }, but got { .str = \"%s\", .len = %zu }",
        ed.number, ed.exponent, ed.number, ed.exponent, len, s.str, s.len
    );
    destroy_string(s);
}

Test(base_json_writer, get_double_exp_as_str_small)
{
    test_get_double_exp_as_str(2.5, 3, 10, "2.500000e3");
}

Test(base_json_writer, get_double_exp_as_str_positive)
{
    test_get_double_exp_as_str(123.456789, 2, 12, "123.456789e2");
}

Test(base_json_writer, get_double_exp_as_str_negative)
{
    test_get_double_exp_as_str(-12345.6789, 5, 15, "-12345.678900e5");
}

/*******************************************************************************
**                               GET_BOOL_AS_STR                              **
*******************************************************************************/
void test_get_bool_as_str(bool b, size_t len, char *nstr)
{
    string_t s = get_bool_as_str(b);
    cr_expect(
        strings_equals(s, STRING_OF(nstr, len)),
        "Expected get_bool_as_str(%s) to return the string { .str = "
        "\"%s\", .len = %zu }, but got { .str = \"%s\", .len = %zu }",
        b ? "true" : "false", b ? "true" : "false", len, s.str, s.len
    );
    destroy_string(s);
}

Test(base_json_writer, get_bool_as_str_true)
{
    test_get_bool_as_str(true, 4, "true");
}

Test(base_json_writer, get_bool_as_str_false)
{
    test_get_bool_as_str(false, 5, "false");
}

/*******************************************************************************
**                               GET_NULL_AS_STR                              **
*******************************************************************************/
Test(base_json_writer, get_null_as_str)
{
    string_t s = get_null_as_str();
    cr_expect(
        strings_equals(s, STRING_OF("null", 4)),
        "Expected get_null_as_str() to return the string { .str = "
        "\"null\", .len = 4 }, but got { .str = \"%s\", .len = %zu }",
        s.str, s.len
    );
    free(s.str);
}
