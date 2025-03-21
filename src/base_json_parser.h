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
// The following MACROS are used int the parsing functions
#define IS_END_CHAR(c)                                                         \
  (!(c) || (c) == ',' || (c) == '\n' || (c) == ']' || (c) == '}')

#define IS_STRING_END(c) (!(c) || ((c) == '"' && prev_c != '\\'))

#define IS_NOT_BOOLEAN(c, l)                                                   \
  (!(l) || ((c) == 'f' && (l) != 5) || ((c) == 't' && (l) != 4))

/**
** \def Used by the functions that read the file using fseef() and fgetc()
**      inside while loops, it sets the cursor on the 'p' position,
**      increments 'p' and puts in 'c' the current character
*/
#define SEEK_AND_GET_CHAR(p)                                                   \
  (!fseek(f, (p)++, SEEK_SET) && (c = fgetc(f)) != EOF)

#ifndef MAX_READ_BUFF_SIZE
#define MAX_READ_BUFF_SIZE (1 << 30) // ~= 1 GB
#endif

#ifndef MAX_NESTED_ARRAYS
#define MAX_NESTED_ARRAYS 255
#endif

#ifndef MAX_NESTED_DICTS
#define MAX_NESTED_DICTS 255
#endif

#define NULL_STR_AND_LEN_TUPLE ((str_and_len_tuple_t){.str = 0, .len = 0})
#define ERROR_LWOWE ((long_with_or_without_exponent_t){.has_exponent = 2})
#define ERROR_DWOWE ((double_with_or_without_exponent_t){.has_exponent = 2})

#define STR_AND_LEN_OF(s, l, f, e)                                             \
  ((str_and_len_tuple_t){                                                      \
      .str = (s), .len = (l), .is_float = (f), .has_exponent = (e)})
#define STR_AND_LEN(s, l)                                                      \
  ((str_and_len_tuple_t){                                                      \
      .str = (s), .len = (l), .is_float = false, .has_exponent = false})
#define STR_AND_LEN_F_OF(s, l)                                                 \
  ((str_and_len_tuple_t){                                                      \
      .str = (s), .len = (l), .is_float = true, .has_exponent = false})
#define STR_AND_LEN_E_OF(s, l)                                                 \
  ((str_and_len_tuple_t){                                                      \
      .str = (s), .len = (l), .is_float = false, .has_exponent = true})
#define STR_AND_LEN_FE_OF(s, l)                                                \
  ((str_and_len_tuple_t){                                                      \
      .str = (s), .len = (l), .is_float = true, .has_exponent = true})

#define EXP_LONG_OF(n, e) ((exp_long_t){.number = (n), .exponent = (e)})
#define EXP_DOUBLE_OF(n, e) ((exp_double_t){.number = (n), .exponent = (e)})

/*******************************************************************************
**                                 STRUCTURES **
*******************************************************************************/
/**
** \typedef str_and_len_tuple_t
** \brief Used by the parse_number() function to return multiple informations
*/
typedef struct {
  char *str;
  size_t len;
  bool is_float;
  bool has_exponent;
} str_and_len_tuple_t;

typedef struct {
  exp_long_t long_exp_value;
  i64 long_value;
  u8 has_exponent; // 0 => false | 1 => true | 2 => error
} long_with_or_without_exponent_t;

typedef struct {
  exp_double_t double_exp_value;
  double double_value;
  u8 has_exponent; // 0 => false | 1 => true | 2 => error
} double_with_or_without_exponent_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Takes sl's char array and transforms it into a long.
**        If the number has an exponent, the exponent is parsed and stored
** \param sl A pointer to an str_and_len_tuple_t struct
** \returns ERROR_LWOWE in case of error, a struct containing the number with
**          and without exponent, as well as a field indicating whether the
**          number has an exponent
*/
long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t *sl);
/**
** \brief Takes sl's char array and transforms it into a double.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an str_and_len_tuple_t object
** \returns ERROR_DWOWE in case of error, a struct containing the number with
**          and without exponent, as well as a field indicating whether the
**          number has an exponent
*/
double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t *sl);

bool is_float(char *str, size_t len);
bool has_exponent(char *str, size_t len);

bool max_nested_arrays_reached(u64 is_in_array);
bool max_nested_dicts_reached(u64 is_in_dict);

/***************************************
**              FUNCTIONS             **
***************************************/
/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param buff The buffer containing the current json file's contents
** \param idx A pointer to the unsigned long containing the index of the '"'
**            that started the string we want to parse
** \returns A NULL_STRING in case of error, the parsed string otherwise
*/
string_t parse_string_buff(char *buff, size_t *idx);
/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param f The file stream
** \param pos A pointer to the unsigned long containing the position of the
**            '"' that started the string we want to parse
** \returns A NULL_STRING in case of error, the parsed string otherwise
*/
string_t parse_string(FILE *f, size_t *pos);

/**
** \brief Reads the buffer from the given pos - 1
** \param buff The buffer containing the current json file's contents
** \param idx The index of the second character of the number (the first one
**            was already read)
** \returns A pointer to the str_and_len_tuple_t struct containing the number
**          as a char array, the length of the char array and whether the number
**          is a float and has an exponent, or NULL_STR_AND_LEN_TUPLE if there
**          was an error
*/
str_and_len_tuple_t parse_number_buff(char *buff, size_t *idx);
/**
** \brief Reads the file from the given pos
** \param f The file stream
** \param pos The pos of the start of the number of which we are currently
**            acquiring the length
** \returns a pointer to the str_and_len_tuple_t struct containing the number as
**          a char array, the length of the char array and whether the number is
**          a float and has an exponent, or NULL_STR_AND_LEN_TUPLE if there
**          was an error
*/
str_and_len_tuple_t parse_number(FILE *f, size_t *pos);

/**
** \param buff The current json file's contents
** \param idx A pointer to the value containing the index of the 't' or 'f' that
**            started the boolean
** \returns 5 if false, 4 if true, 0 otherwise
*/
size_t parse_boolean_buff(char *buff, size_t *idx);
/**
** \param f The file stream
** \param pos A pointer to the value containing the index of the 't' or 'f' that
**            started the boolean
** \returns 5 if false, 4 if true, 0 otherwise
*/
size_t parse_boolean(FILE *f, size_t *pos);

/**
** \param idx The index of the character just after the '[' that begins the
**            current array
** \returns The number of elements of the current array
*/
size_t get_nb_elts_array_buff(char *buff, size_t idx);
/**
** \param f The file stream
** \param pos The position in the file of the character just after the '['
**            that begins the current array
** \returns The number of elements of the current array
*/
size_t get_nb_elts_array(FILE *f, size_t pos);

/**
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character just after the '{' that begins the
**            current dict
** \returns The number of elements of the current dict
*/
size_t get_nb_elts_dict_buff(char *buff, size_t idx);
/**
** \param f The file stream
** \param pos The position in the file of the character just after the '{'
**            that begins the current dict
** \returns The number of elements of the current dict
*/
size_t get_nb_elts_dict(FILE *f, size_t pos);

/**
** \param f The file stream
** \param pos The position in the file of the character after the '[' that
**            begins the current array
** \returns The total number of characters in the current array - 1 (the
**          first '[' is not counted)
*/
size_t get_nb_chars_in_array(FILE *f, size_t pos);

/**
** \param f The file stream
** \param pos The position in the file of the character after the '{' that
**            begins the current dict
** \returns The total number of characters in the current dict - 1 (the
**          first '{' is not counted)
*/
size_t get_nb_chars_in_dict(FILE *f, size_t pos);

#endif // !BASE_JSON_PARSER_H
