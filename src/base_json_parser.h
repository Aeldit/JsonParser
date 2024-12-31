#ifndef BASE_JSON_PARSER_H
#define BASE_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdio.h>

#include "base_json_storage.h"
#include "ro_json_storage.h"

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
#define IS_NUMBER_START(c) (('0' <= (c) && (c) <= '9') || (c) == '-')
#define IS_BOOL_START(c) ((c) == 't' || (c) == 'f')

#define IS_END_CHAR(c)                                                         \
    ((c) == 0 || (c) == ',' || (c) == '\n' || (c) == ']' || (c) == '}')

#define IS_STRING_END(c) ((c) == 0 || ((c) == '"' && prev_c != '\\'))

#define IS_NOT_BOOLEAN(c, l)                                                   \
    ((l) == 0 || ((c) == 'f' && (l) != 5) || ((c) == 't' && (l) != 4))

/**
** \def Used by the functions that read the file using fseef() and fgetc()
**      inside while loops, it sets the cursor on the 'p' position,
**      increments 'p' and puts in 'c' the current character
*/
#define SEEK_AND_GET_CHAR(p) (!fseek(f, p++, SEEK_SET) && (c = fgetc(f)) != EOF)

#ifndef MAX_READ_BUFF_SIZE
#    define MAX_READ_BUFF_SIZE (1 << 30) // ~= 1 GB
#endif

#ifndef MAX_NESTED_ARRAYS
#    define MAX_NESTED_ARRAYS 255
#endif

#ifndef MAX_NESTED_DICTS
#    define MAX_NESTED_DICTS 255
#endif

#define NULL_STR_AND_LEN_TUPLE ((str_and_len_tuple_t){ .str = 0, .len = 0 })
#define ERROR_LWOWE ((long_with_or_without_exponent_t){ .has_exponent = 2 })
#define ERROR_DWOWE ((double_with_or_without_exponent_t){ .has_exponent = 2 })

#define STR_AND_LEN_OF(s, l, float, exponent)                                  \
    ((str_and_len_tuple_t){                                                    \
        .str = s, .len = l, .is_float = float, .has_exponent = exponent })

/*******************************************************************************
**                                 STRUCTURES **
*******************************************************************************/
/**
** \typedef str_and_len_tuple_t
** \brief Used by the parse_number() function to return multiple
*informations
*/
typedef struct
{
    char *str;
    unsigned len;
    char is_float;
    char has_exponent;
} str_and_len_tuple_t;

typedef struct
{
    exponent_long_t long_exp_value;
    long long_value;
    char has_exponent; // 0 => false | 1 => true | 2 => error
} long_with_or_without_exponent_t;

typedef struct
{
    exponent_double_t double_exp_value;
    double double_value;
    char has_exponent; // 0 => false | 1 => true | 2 => error
} double_with_or_without_exponent_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Takes sl's char array and transforms it into an unsigned.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an str_and_len_tuple_t struct
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t *sl);

/**
** \brief Takes sl's char array and transforms it into a double.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an str_and_len_tuple_t object
** \returns The 0 in case of error (or if the number was 0), the number
**          otherwise
*/
double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t *sl);

char is_float(char *str, unsigned long len);
char has_exponent(char *str, unsigned long len);

char max_nested_arrays_reached(long is_in_array);
char max_nested_dicts_reached(long is_in_dict);

/***************************************
**              FUNCTIONS             **
***************************************/
/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param buff The buffer containing the current json file or object
** \param idx A pointer to the unsigned long containing the index of the '"'
**            that started the string we want to parse
** \returns An empty string in case of error, the parsed string otherwise
*/
string_t parse_string_buff(char *buff, unsigned long *idx);
/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param f The file stream
** \param pos A pointer to the unsigned long containing the position of the
**            '"' that started the string we want to parse
** \returns An empty string in case of error, the parsed string otherwise
*/
string_t parse_string(FILE *f, unsigned long *pos);

/**
** \brief Reads the buffer from the given pos - 1
** \param buff The buffer containing the current json file or object
** \param idx The index of the second character of the number (the first one
**            was already read)
** \returns A pointer to the str_and_len_tuple_t struct containing the number
**          as a char array, the length of the char array and whether the number
**          is a float and has an exponent
*/
str_and_len_tuple_t parse_number_buff(char *buff, unsigned long *idx);
/**
** \brief Reads the file from the given pos and returns a pointer to the
**        str_and_len_tuple_t struct containing the number as a char array, the
**        length of the char array and whether the number is a float and
**        has an exponent
** \param f The file stream
** \param pos The pos of the start of the number of which we are currently
**            acquiring the length
*/
str_and_len_tuple_t parse_number(FILE *f, unsigned long *pos);

/**
** \returns 5 if false, 4 if true, 0 otherwise
**/
unsigned long parse_boolean_buff(char *buff, unsigned long *idx);
unsigned long parse_boolean(FILE *f, unsigned long *pos);

/**
** \param idx The index of the character just after the '[' that begins the
**            current array
** \returns The number of elements of the current array
*/
unsigned long get_nb_elts_array_buff(char *buff, unsigned long idx);
/**
** \param pos The position in the file of the character just after the '['
**            that begins the current array
** \returns The number of elements of the current array
*/
unsigned long get_nb_elts_array(FILE *f, unsigned long pos);

/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character just after the '{' that begins the
**            current dict
** \returns The number of elements of the current dict
*/
unsigned long get_nb_elts_dict_buff(char *buff, unsigned long idx);
/**
** \param f The file stream
** \param pos The position in the file of the character just after the '{'
**            that begins the current dict
** \returns The number of elements of the current dict
*/
unsigned long get_nb_elts_dict(FILE *f, unsigned long pos);

/**
** \param f The file stream
** \param pos The position in the file of the character after the '[' that
**            begins the current array
** \param err A pointer to a char that will be updated if an error occurs
**            (we return the number of chars, so we can't use a number to
**            indicate an error)
** \returns The total number of characters in the current array - 1 (the
**          first '[' is not counted)
*/
unsigned long get_nb_chars_in_array(FILE *f, unsigned long pos);

/**
** \param f The file stream
** \param pos The position in the file of the character after the '{' that
**            begins the current dict
** \returns The total number of characters in the current dict - 1 (the
**          first '{' is not counted)
*/
unsigned long get_nb_chars_in_dict(FILE *f, unsigned long pos);

#endif // !BASE_JSON_PARSER_H
