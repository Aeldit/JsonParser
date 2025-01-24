#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/rw_json_equality.h"

/*******************************************************************************
**                                ARRAYS_EQUAL                                **
*******************************************************************************/
void rw_test_arrays_equal(rw_array_t *a, rw_array_t *b, char expected_is_equal)
{
    char is_equal = rw_arrays_equal(a, b);

    cr_expect(is_equal == expected_is_equal,
              "Expected arrays_equal(a, b) to be %s, but it was %s",
              expected_is_equal ? "true" : "false",
              is_equal ? "true" : "false");
}

Test(rw_equality, arrays_equal_true)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 1);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_size_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_strings_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("teet"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_longs_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 12345678);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_doubles_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 0.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_exp_longs_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 2));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_exp_doubles_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.2556, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_bools_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 1);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_arrays_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());

    rw_array_add_dict(a, init_rw_dict());
    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_dicts_returns_false)
{
    rw_array_t *a = init_rw_array();
    rw_array_t *b = init_rw_array();

    rw_array_add_str(a, string_nofree_of("test"));
    rw_array_add_str(b, string_nofree_of("test"));

    rw_array_add_long(a, 123456789);
    rw_array_add_long(b, 123456789);

    rw_array_add_double(a, 1.53685);
    rw_array_add_double(b, 1.53685);

    rw_array_add_exp_long(a, EXP_LONG_OF(2, 3));
    rw_array_add_exp_long(b, EXP_LONG_OF(2, 3));

    rw_array_add_exp_double(a, EXP_DOUBLE_OF(1.25563, 5));
    rw_array_add_exp_double(b, EXP_DOUBLE_OF(1.25563, 5));

    rw_array_add_bool(a, 0);
    rw_array_add_bool(b, 0);

    rw_array_add_null(a);
    rw_array_add_null(b);

    rw_array_add_array(a, init_rw_array());
    rw_array_add_array(b, init_rw_array());

    rw_array_add_dict(b, init_rw_dict());

    rw_test_arrays_equal(a, b, 0);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

/*******************************************************************************
**                                 DICTS_EQUAL **
*******************************************************************************/
void rw_test_dicts_equal(rw_dict_t *a, rw_dict_t *b, char expected_is_equal)
{
    char is_equal = rw_dicts_equal(a, b);

    cr_expect(is_equal == expected_is_equal,
              "Expected dicts_equal(a, b) to be %s, but it was %s",
              expected_is_equal ? "true" : "false",
              is_equal ? "true" : "false");
}

Test(rw_equality, dicts_equal_true)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 1);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_strings)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("ste"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_longs)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 12345678);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_doubles)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 0.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_exp_longs)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 2));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_exp_doubles)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(0.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_bools)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 1);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_arrays)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());
    rw_dict_add_dict(b, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_dicts)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    rw_dict_add_str(a, string_nofree_of("str"), string_nofree_of("test"));
    rw_dict_add_str(b, string_nofree_of("str"), string_nofree_of("test"));

    rw_dict_add_long(a, string_nofree_of("long"), 123456789);
    rw_dict_add_long(b, string_nofree_of("long"), 123456789);

    rw_dict_add_double(a, string_nofree_of("double"), 1.53685);
    rw_dict_add_double(b, string_nofree_of("double"), 1.53685);

    rw_dict_add_exp_long(a, string_nofree_of("expl"), EXP_LONG_OF(2, 3));
    rw_dict_add_exp_long(b, string_nofree_of("expl"), EXP_LONG_OF(2, 3));

    rw_dict_add_exp_double(a, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));
    rw_dict_add_exp_double(b, string_nofree_of("expd"),
                           EXP_DOUBLE_OF(1.25563, 5));

    rw_dict_add_bool(a, string_nofree_of("bool"), 0);
    rw_dict_add_bool(b, string_nofree_of("bool"), 0);

    rw_dict_add_null(a, string_nofree_of(""));
    rw_dict_add_null(b, string_nofree_of(""));

    rw_dict_add_array(a, string_nofree_of("array"), init_rw_array());
    rw_dict_add_array(b, string_nofree_of("array"), init_rw_array());

    rw_dict_add_dict(a, string_nofree_of("dict"), init_rw_dict());

    rw_test_dicts_equal(a, b, 0);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}
