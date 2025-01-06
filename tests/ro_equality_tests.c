#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/ro_equality.h"

/*******************************************************************************
**                               EXP_LONG_EQUALS                              **
*******************************************************************************/
void test_exp_long_equals(exponent_long_t a, exponent_long_t b,
                          char expected_is_equal)
{
    char is_equal = exp_long_equals(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected exp_long_equals(%lde%ld, %lde%ld) to be %s, but it was %s",
        a.number, a.exponent, b.number, b.exponent,
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false");
}

Test(ro_equality, exp_long_equals_numtrue_exptrue)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(2, 3), 1);
}

Test(ro_equality, exp_long_equals_numtrue_expfalse)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(2, 2), 0);
}

Test(ro_equality, exp_long_equals_numfalse_exptrue)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(3, 3), 0);
}

Test(ro_equality, exp_long_equals_numfalse_expfalse)
{
    test_exp_long_equals(EXP_LONG_OF(1, 2), EXP_LONG_OF(3, 4), 0);
}

/*******************************************************************************
**                              EXP_DOUBLE_EQUALS                             **
*******************************************************************************/
void test_exp_double_equals(exponent_double_t a, exponent_double_t b,
                            char expected_is_equal)
{
    char is_equal = exp_double_equals(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected exp_double_equals(%lfe%ld, %lfe%ld) to be %s, but it was %s",
        a.number, a.exponent, b.number, b.exponent,
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false");
}

Test(ro_equality, exp_double_equals_numtrue_exptrue)
{
    test_exp_double_equals(EXP_DOUBLE_OF(2.5, 3), EXP_DOUBLE_OF(2.5, 3), 1);
}

Test(ro_equality, exp_double_equals_numtrue_expfalse)
{
    test_exp_double_equals(EXP_DOUBLE_OF(2.5, 2), EXP_DOUBLE_OF(2.5, 3), 0);
}

Test(ro_equality, exp_double_equals_numfalse_exptrue)
{
    test_exp_double_equals(EXP_DOUBLE_OF(2.5, 3), EXP_DOUBLE_OF(2.45, 3), 0);
}

Test(ro_equality, exp_double_equals_numfalse_expfalse)
{
    test_exp_double_equals(EXP_DOUBLE_OF(2.5, 3), EXP_DOUBLE_OF(2.45, 4), 0);
}

/*******************************************************************************
**                                ARRAYS_EQUAL                                **
*******************************************************************************/
void test_arrays_equal(ro_array_t *a, ro_array_t *b, char expected_is_equal)
{
    char is_equal = ro_arrays_equal(a, b);

    cr_expect(is_equal == expected_is_equal,
              "Expected arrays_equal(a, b) to be %s, but it was %s",
              expected_is_equal ? "true" : "false",
              is_equal ? "true" : "false");
}

Test(ro_equality, arrays_equal_true)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 1);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_size_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(8);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_strings_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("teet"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_longs_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 12345678);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_doubles_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 0.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_exp_longs_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 2));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_exp_doubles_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.2556, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_bools_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 1);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_arrays_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(2));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_dicts_returns_false)
{
    ro_array_t *a = init_ro_array(9);
    ro_array_t *b = init_ro_array(9);

    ro_array_add_str(a, string_nofree_of("test"));
    ro_array_add_str(b, string_nofree_of("test"));

    ro_array_add_long(a, 123456789);
    ro_array_add_long(b, 123456789);

    ro_array_add_double(a, 1.53685);
    ro_array_add_double(b, 1.53685);

    ro_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    ro_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    ro_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    ro_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    ro_array_add_bool(a, 0);
    ro_array_add_bool(b, 0);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(4));

    test_arrays_equal(a, b, 0);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

/*******************************************************************************
**                                 DICTS_EQUAL                                **
*******************************************************************************/
void test_dicts_equal(ro_dict_t *a, ro_dict_t *b, char expected_is_equal)
{
    char is_equal = ro_dicts_equal(a, b);

    cr_expect(is_equal == expected_is_equal,
              "Expected dicts_equal(a, b) to be %s, but it was %s",
              expected_is_equal ? "true" : "false",
              is_equal ? "true" : "false");
}

Test(ro_equality, dicts_equal_true)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 1);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_strings)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("ste"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_longs)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 12345678);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_doubles)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 0.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_exp_longs)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 2));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_exp_doubles)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(0.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_bools)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 1);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_arrays)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(1));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(50));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(0));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_dicts)
{
    ro_dict_t *a = init_ro_dict(9);
    ro_dict_t *b = init_ro_dict(9);

    ro_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    ro_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    ro_dict_add_long(a, string_nofree_of("long"), 123456789);
    ro_dict_add_long(b, string_nofree_of("long"), 123456789);

    ro_dict_add_double(a, string_nofree_of("double"), 1.53685);
    ro_dict_add_double(b, string_nofree_of("double"), 1.53685);

    ro_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    ro_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    ro_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    ro_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    ro_dict_add_bool(a, string_nofree_of("bool"), 0);
    ro_dict_add_bool(b, string_nofree_of("bool"), 0);

    ro_dict_add_null(a, string_nofree_of(""));
    ro_dict_add_null(b, string_nofree_of(""));

    ro_dict_add_array(a, string_nofree_of("array"), init_ro_array(0));
    ro_dict_add_array(b, string_nofree_of("array"), init_ro_array(0));

    ro_dict_add_dict(a, string_nofree_of("dict"), init_ro_dict(5));
    ro_dict_add_dict(b, string_nofree_of("dict"), init_ro_dict(0));

    test_dicts_equal(a, b, 0);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}
