#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/ro_json_equality.h"

/*******************************************************************************
**                                ARRAYS_EQUAL                                **
*******************************************************************************/
void ro_test_arrays_equal(ro_array_t *a, ro_array_t *b, bool expected_is_equal)
{
    bool is_equal = ro_arrays_equal(a, b);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, true);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, true);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(2));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(0));

    ro_test_arrays_equal(a, b, false);

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

    ro_array_add_bool(a, false);
    ro_array_add_bool(b, false);

    ro_array_add_null(a);
    ro_array_add_null(b);

    ro_array_add_array(a, init_ro_array(0));
    ro_array_add_array(b, init_ro_array(0));

    ro_array_add_dict(a, init_ro_dict(0));
    ro_array_add_dict(b, init_ro_dict(4));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

/*******************************************************************************
**                                 DICTS_EQUAL                                **
*******************************************************************************/
void ro_test_dicts_equal(ro_dict_t *a, ro_dict_t *b, bool expected_is_equal)
{
    bool is_equal = ro_dicts_equal(a, b);

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

    ro_test_dicts_equal(a, b, true);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

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

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}
