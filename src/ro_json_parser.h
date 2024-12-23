#ifndef RO_JSON_PARSER_H
#define RO_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "ro_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Parse the given file and returns the associated JSON object
*/
ro_json_t *ro_parse(char *file);

#endif // !RO_JSON_PARSER_H
