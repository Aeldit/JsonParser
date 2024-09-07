#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>

#include "values_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \param f The file stream
** \param pos The pos of the character just after the '[' that begins the
**            current array
** \returns The json array parsed at the pos
*/
json_array_st *parse_array(storage_st *s, FILE *f, uint64_t *pos);

/**
** \param f The file stream
** \param pos The pos of the character just after the '[' that begins the
**            current array
** \returns The json dict parsed at the pos
*/
json_dict_st *parse_json_dict(storage_st *s, FILE *f, uint64_t *pos);

#endif // !JSON_PARSER_H
