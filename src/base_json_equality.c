#include "base_json_equality.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
inline char exp_long_equals(exponent_long_t a, exponent_long_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}

inline char exp_double_equals(exponent_double_t a, exponent_double_t b)
{
    return a.number == b.number && a.exponent == b.exponent;
}
