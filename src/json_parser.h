#ifndef PARSER_H
#define PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Parse the given file and returns the associated JSON object
*/
JSON *parse(char *file);

#endif // !PARSER_H