// clang-format Language: C
#ifndef RW_JSON_EQUALITY_H
#define RW_JSON_EQUALITY_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "rw_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
bool rw_arrays_equal(rw_array_t *a, rw_array_t *b);
bool rw_dicts_equal(rw_dict_t *a, rw_dict_t *b);
bool rw_json_equal(rw_json_t *a, rw_json_t *b);

#endif // !RW_JSON_EQUALITY_H
