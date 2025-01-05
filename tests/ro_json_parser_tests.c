#include <criterion/criterion.h>

#include "../src/ro_equality.h"

#define EXP_LONG_OF(n, e) ((exponent_long_t){ .number = n, .exponent = e })
#define EXP_DOUBLE_OF(n, e) ((exponent_double_t){ .number = n, .exponent = e })

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
    test_exp_long_equals(EXP_LONG_OF(2, 3), EXP_LONG_OF(2, 2), 1);
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
