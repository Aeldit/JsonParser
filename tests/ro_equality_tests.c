#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/ro_json_equality.h"
#include "../src/ro_json_storage.h"

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
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, true);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_size_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)));

    ro_array_t *b = init_ro_array_with(
        8, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_strings_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("teet")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_longs_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(12345678),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_doubles_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(0.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_exp_longs_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 2),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_exp_doubles_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.2556, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_bools_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(true), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_arrays_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(2)), ROVAL_DICT(init_ro_dict(0)));

    ro_test_arrays_equal(a, b, false);

    destroy_ro_array(a);
    destroy_ro_array(b);
}

Test(ro_equality, arrays_equal_diff_dicts_returns_false)
{
    ro_array_t *a = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(0)));

    ro_array_t *b = init_ro_array_with(
        9, ROVAL_STR(string_nofree_of("test")), ROVAL_LONG(123456789),
        ROVAL_DOUBLE(1.53685), ROVAL_EXPLONG_T(2, 3),
        ROVAL_EXPDOUBLE_T(1.25563, 5), ROVAL_DOUBLE(false), ROVAL_NULL,
        ROVAL_ARR(init_ro_array(0)), ROVAL_DICT(init_ro_dict(4)));

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
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, true);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_strings)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("ste"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_longs)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 12345678),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_doubles)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 0.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_exp_longs)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 2)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_exp_doubles)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(0.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_bools)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), true),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_arrays)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(1)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(50)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}

Test(ro_equality, dicts_equal_diff_dicts)
{
    ro_dict_t *a = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(5)));

    ro_dict_t *b = init_ro_dict_with(
        9, ROIT_STR(string_nofree_of("str"), string_nofree_of("test")),
        ROIT_LONG(string_nofree_of("long"), 123456789),
        ROIT_DOUBLE(string_nofree_of("double"), 1.53685),
        ROIT_EXPLONG(string_nofree_of("expl"), EXP_LONG_OF(2, 3)),
        ROIT_EXPDOUBLE(string_nofree_of("expd"), EXP_DOUBLE_OF(1.25563, 5)),
        ROIT_BOOL(string_nofree_of("bool"), false),
        ROIT_NULL(string_nofree_of("")),
        ROIT_ARR(string_nofree_of("array"), init_ro_array(0)),
        ROIT_DICT(string_nofree_of("dict"), init_ro_dict(0)));

    ro_test_dicts_equal(a, b, false);

    destroy_ro_dict(a);
    destroy_ro_dict(b);
}
