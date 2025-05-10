#include <criterion/criterion.h>

#include "../src/base_json_parser.h"
#include "../src/base_json_storage.h"

/*******************************************************************************
**                               EXP_LONG_EQUALS                              **
*******************************************************************************/
void test_exp_long_equals(exp_long_t a, exp_long_t b, bool expected_is_equal)
{
    bool is_equal = exp_long_equals(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected exp_long_equals(%lde%ld, %lde%ld) to be %s, but it was %s",
        a.number, a.exponent, b.number, b.exponent,
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false"
    );
}

Test(ro_equality, exp_long_equals_numtrue_exptrue)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(2, 3), true);
}

Test(ro_equality, exp_long_equals_numtrue_expfalse)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(2, 2), false);
}

Test(ro_equality, exp_long_equals_numfalse_exptrue)
{
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(3, 3), false);
}

Test(ro_equality, exp_long_equals_numfalse_expfalse)
{
    test_exp_long_equals(EXP_LONG_OF(1, 2), EXP_LONG_OF(3, 4), false);
}

/*******************************************************************************
**                              EXP_DOUBLE_EQUALS                             **
*******************************************************************************/
void test_exp_double_equals(
    exp_double_t a, exp_double_t b, bool expected_is_equal
)
{
    bool is_equal = exp_double_equals(a, b);

    cr_expect(
        is_equal == expected_is_equal,
        "Expected exp_double_equals(%lfe%ld, %lfe%ld) to be %s, but it was %s",
        a.number, a.exponent, b.number, b.exponent,
        expected_is_equal ? "true" : "false", is_equal ? "true" : "false"
    );
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
