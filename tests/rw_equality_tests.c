#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/rw_json_equality.h"
#include "../src/rw_json_storage.h"

/*******************************************************************************
**                                ARRAYS_EQUAL                                **
*******************************************************************************/
void rw_test_arrays_equal(rw_array_t *a, rw_array_t *b, bool expected_is_equal)
{
    bool is_equal = rw_arrays_equal(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected arrays_equal(a, b) to be %s, but it was %s",
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false"
    );
}

Test(rw_equality, arrays_equal_true)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, true);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_size_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_NULL
    );
    rw_array_t *b = init_rw_array_with(
        8, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_strings_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("teet")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_longs_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(12345678),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_doubles_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(0.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_exp_longs_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 2),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_exp_doubles_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.2556, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_bools_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(true), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_arrays_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array_with(2, RWVAL_NULL, RWVAL_NULL)),
        RWVAL_DICT(init_rw_dict())
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

Test(rw_equality, arrays_equal_diff_dicts_returns_false)
{
    rw_array_t *a = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()), RWVAL_DICT(init_rw_dict())
    );
    rw_array_t *b = init_rw_array_with(
        9, RWVAL_STR(string_nofree_of("test")), RWVAL_LONG(123456789),
        RWVAL_DOUBLE(1.53685), RWVAL_EXPLONG_T(2, 3),
        RWVAL_EXPDOUBLE_T(1.25563, 5), RWVAL_DOUBLE(false), RWVAL_NULL,
        RWVAL_ARR(init_rw_array()),
        RWVAL_DICT(init_rw_dict_with(
            3, RWIT_LONG(string_nofree_of("1"), 5),
            RWIT_NULL(string_nofree_of("aesr")),
            RWIT_NULL(string_nofree_of("nnnn"))
        ))
    );

    rw_test_arrays_equal(a, b, false);

    destroy_rw_array(a);
    destroy_rw_array(b);
}

/*******************************************************************************
**                                 DICTS_EQUAL                                **
*******************************************************************************/
void rw_test_dicts_equal(rw_dict_t *a, rw_dict_t *b, bool expected_is_equal)
{
    bool is_equal = rw_dicts_equal(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected dicts_equal(a, b) to be %s, but it was %s",
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false"
    );
}

Test(rw_equality, empty_dicts)
{
    rw_dict_t *a = init_rw_dict();
    rw_dict_t *b = init_rw_dict();

    cr_expect(
        rw_dicts_equal(a, b),
        "Expected 2 empty dicts to be equal, but they weren't"
    );

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_true)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, true);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_strings)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("ste"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_longs)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 12345678),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_doubles)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 0.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_exp_longs)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 2)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_exp_doubles)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(0.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_bools)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), true),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_arrays)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array_with(1, RWVAL_NULL)),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(
            string_nofree_of("array"),
            init_rw_array_with(1, RWVAL_STR(string_nofree_of("test")))
        ),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}

Test(rw_equality, dicts_equal_diff_dicts)
{
    rw_dict_t *a = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(
            string_nofree_of("dict"),
            init_rw_dict_with(1, RWIT_NULL(string_nofree_of("bla")))
        )
    );
    rw_dict_t *b = init_rw_dict_with(
        9, RWIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        RWIT_LONG(string_nofree_of("long"), 123456789),
        RWIT_DOUBLE(string_nofree_of("double"), 1.53685),
        RWIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        RWIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        RWIT_BOOL(string_nofree_of("bool"), false),
        RWIT_NULL(string_nofree_of("")),
        RWIT_ARR(string_nofree_of("array"), init_rw_array()),
        RWIT_DICT(string_nofree_of("dict"), init_rw_dict())
    );

    rw_test_dicts_equal(a, b, false);

    destroy_rw_dict(a);
    destroy_rw_dict(b);
}
