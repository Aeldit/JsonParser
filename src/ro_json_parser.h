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
/**
** \param b The buffer containing the json currently being parsed
** \param idx The index of the character '[' that begins the current array
** \returns The json array parsed from the position
*/
ro_array_t *ro_parse_array_buff(char *b, unsigned long *idx);
/**
** \param f The file stream
** \param pos The postion in the file of the character after the '[' that
**            begins the current array
** \returns The json ro_array_t parsed from the position
*/
ro_array_t *ro_parse_array(FILE *f, unsigned long *pos);

/**
** \param b The buffer containing the json currently being parsed
** \param idx A pointer to the index of the character '{' that begins the
**            current dict.
**            If the given pointer is a nullptr, it means that the buffer is
**            a new one created just before calling this function, meaning the
**            index starts at 0
** \returns The json dict parsed from the index
*/
ro_dict_t *ro_parse_dict_buff(char *b, unsigned long *idx);
/**
** \param f The file stream
** \param pos A pointer to the position in the file of the character after
**            the '{' that begins the current dict
** \returns The json dict parsed from the position
*/
ro_dict_t *ro_parse_dict(FILE *f, unsigned long *pos);

/**
** \brief Parses the given file and returns the associated ro_json_t structure
*/
ro_json_t *ro_parse(char *file);

#endif // !RO_JSON_PARSER_H
