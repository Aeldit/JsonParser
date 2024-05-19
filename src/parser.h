#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "json.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define BEGIN_OBJECT '{'
#define END_OBJECT '}'
#define BEGIN_ARRAY '['
#define END_ARRAY ']'
#define NAME_SEPARATOR ':'
#define VALUE_SEPARATOR ','

#define WRITE_BUFF_LEN 512

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct states
{
    char is_in_json;
    char is_in_array;
    char is_in_string;
    char is_waiting_key;
    char is_waiting_value;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_dict_st *parse(char *file);

#endif // !JSON_PARSER_H
