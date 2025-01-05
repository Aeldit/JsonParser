#ifndef JSON_EQUALITY_H
#define JSON_EQUALITY_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_storage.h"
#include "ro_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char exp_long_equals(exponent_long_t a, exponent_long_t b);
char exp_double_equals(exponent_double_t a, exponent_double_t b);
char arrays_equal(ro_array_t a, ro_array_t b);
char dicts_equal(ro_dict_t a, ro_dict_t b);

#endif // !JSON_EQUALITY_H
