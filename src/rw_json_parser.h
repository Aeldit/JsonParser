#ifndef RW_JSON_PARSER_H
#define RW_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "rw_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Parse the given file and returns the associated JSON object
*/
rw_json_t *rw_parse(char *file);

#endif // !RW_JSON_PARSER_H
