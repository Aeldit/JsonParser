#ifndef RO_JSON_PARSER_H
#define RO_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>

#include "ro_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
ro_array_t *ro_parse_array_buff(char *b, unsigned long *idx);
ro_array_t *ro_parse_array(FILE *f, unsigned long *pos);

ro_dict_t *ro_parse_dict_buff(char *b, unsigned long *idx);
ro_dict_t *ro_parse_dict(FILE *f, unsigned long *pos);

/**
** \brief Parse the given file and returns the associated JSON object
*/
ro_json_t *ro_parse(char *file);

#endif // !RO_JSON_PARSER_H
