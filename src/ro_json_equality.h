// clang-format Language: C
#ifndef RO_JSON_EQUALITY_H
#define RO_JSON_EQUALITY_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "ro_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
bool ro_arrays_equal(ro_array_t a, ro_array_t b);
bool ro_dicts_equal(ro_dict_t a, ro_dict_t b);
bool ro_json_equal(ro_json_t a, ro_json_t b);

#endif // !RO_JSON_EQUALITY_H
