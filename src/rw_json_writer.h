// clang-format Language: C
#ifndef RW_JSON_WRITER_H
#define RW_JSON_WRITER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "rw_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void write_rw_json_to_file(rw_json_t j, char *file_name);

void rw_array_print(rw_array_t a);
void rw_dict_print(rw_dict_t d);

#endif // !RW_JSON_WRITER_H
