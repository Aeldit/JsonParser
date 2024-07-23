#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>

#include "json.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct states
{
    char is_in_json;
    char is_in_array;
    char is_in_str;
    char is_in_key;
    char is_in_value;
    char is_waiting_key;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *parse_json_dict(FILE *f, uint64_t *pos);

json_dict_st *parse(char *file);

#endif // !JSON_PARSER_H
