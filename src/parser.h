#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
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

struct json
{
    char is_array;
    json_dict_st *jd;
    json_array_st *ja;
};

typedef struct json json_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_st parse(char *file);

#endif // !JSON_PARSER_H
