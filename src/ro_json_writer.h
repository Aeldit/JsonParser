#ifndef RO_JSON_WRITE_H
#define RO_JSON_WRITE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "ro_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void write_ro_json_to_file(ro_json_t j, char *file_name);

void ro_array_print(ro_array_t a);
void ro_dict_print(ro_dict_t d);

#endif // !RO_JSON_WRITE_H
