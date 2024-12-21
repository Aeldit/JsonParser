#ifndef BASE_JSON_STORAGE_H
#define BASE_JSON_STORAGE_H

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define T_ERROR 0
#define T_STR 1
#define T_INT 2
#define T_DOUBLE 3
#define T_BOOL 4
#define T_NULL 5
#define T_ARR 6
#define T_DICT 7

#define STRING_OF(string, len) ((string_t){ .str = (string), .length = (len) })
#define EMPTY_STRING ((string_t){ .str = 0, .length = 0 })

/*
** \def Checks if the JSON instance is not null, if the JSON instance is an
**      array and if the array is not null
*/
#define IS_ARRAY(j) ((j) && (j->is_array) && (j->array))

/*
** \def Checks if the JSON instance is not null, if the JSON instance is a dict
**      and if the dict is not null
*/
#define IS_DICT(j) ((j) && !(j->is_array) && (j->dict))

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct
{
    char *str;
    unsigned length;
} string_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char strings_equals(string_t s1, string_t s2);

#endif // !BASE_JSON_STORAGE_H
