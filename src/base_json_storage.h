#ifndef BASE_JSON_STORAGE_H
#define BASE_JSON_STORAGE_H

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define T_ERROR 0
#define T_STR 1
#define T_LONG 2
#define T_DOUBLE 3
#define T_EXP_LONG 4
#define T_EXP_DOUBLE 5
#define T_BOOL 6
#define T_NULL 7
#define T_ARR 8
#define T_DICT 9

#define STRING_OF(s, l)                                                        \
    ((string_t){ .str = (s), .len = (l), .needs_freeing = 1 })
#define STRING_NOFREE_OF(s, l)                                                 \
    ((string_t){ .str = (s), .len = (l), .needs_freeing = 0 })
#define NULL_STRING ((string_t){ .str = 0 })

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
    unsigned len;
    char needs_freeing;
} string_t;

typedef struct
{
    long number;
    long exponent;
} exponent_long_t;

typedef struct
{
    double number;
    long exponent;
} exponent_double_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
string_t string_of(char *s);
string_t string_nofree_of(char *s);

char strings_equals(string_t s1, string_t s2);

/**
** \brief If the string is not null but its length is 0, it means that it
**        was not allocated so we don't free it (added with STRING_OF("\0", 0)).
**        Otherwise, we free the char array
*/
void destroy_string(string_t s);

#endif // !BASE_JSON_STORAGE_H
