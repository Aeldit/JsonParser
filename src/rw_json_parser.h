#ifndef RW_JSON_PARSER_H
#define RW_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>

#include "rw_json_storage.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character '[' that begins the current rw_array_t
** \returns The json rw_array_t parsed from the position
*/
rw_array_t *rw_parse_array_buff(char *b, size_t *idx);
/**
** \param f The file stream
** \param pos The postion in the file of the character after the '[' that
**            begins the current rw_array_t
** \returns The json rw_array_t parsed from the position
*/
rw_array_t *rw_parse_array(FILE *f, size_t *pos);

/**
** \param b The buffer containing the object currently being parsed
** \param idx A pointer to the index of the character '{' that begins the
**            current dict.
**            If the given pointer is a nullptr, it means that the buffer is
*a
**            new one created just before calling this function, meaning the
**            index starts at 0
** \returns The json dict parsed from the index
*/
rw_dict_t *rw_parse_dict_buff(char *b, size_t *idx);
/**
** \param f The file stream
** \param pos A pointer to the position in the file of the character after
*the
**            '{' that begins the current dict
** \returns The json dict parsed from the position
*/
rw_dict_t *rw_parse_dict(FILE *f, size_t *pos);

/**
** \brief Parse the given file and returns the associated JSON object
*/
rw_json_t *rw_parse(char *file);

#endif // !RW_JSON_PARSER_H
