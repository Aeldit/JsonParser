#ifndef BASE_JSON_PARSER_H
#define BASE_JSON_PARSER_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "base_json_storage.h"
#include "ro_json_storage.h"

/*******************************************************************************
**                                   MACROS                                   **
*******************************************************************************/
#ifndef MAX_READ_BUFF_SIZE
#    define MAX_READ_BUFF_SIZE (1 << 30) // ~= 1 GB
#endif

#define NULL_STR_AND_LEN_TUPLE                                                 \
    ((str_and_len_tuple_t){                                                    \
        .str          = 0,                                                     \
        .len          = 0,                                                     \
        .is_float     = false,                                                 \
        .has_exponent = false,                                                 \
    })
#define ERROR_LWOWE                                                            \
    ((long_with_or_without_exponent_t){                                        \
        .long_exp_value = (exp_long_t){ .number = 0, .exponent = 0 },          \
        .has_exponent   = 2,                                                   \
    })
#define ERROR_DWOWE                                                            \
    ((double_with_or_without_exponent_t){                                      \
        .double_exp_value = (exp_double_t){ .number = 0, .exponent = 0 },      \
        .has_exponent     = 2,                                                 \
    })

#define STR_AND_LEN_OF(s, l, f, e)                                             \
    ((str_and_len_tuple_t                                                      \
    ){ .str = (s), .len = (l), .is_float = (f), .has_exponent = (e) })
#define STR_AND_LEN(s, l)                                                      \
    ((str_and_len_tuple_t                                                      \
    ){ .str = (s), .len = (l), .is_float = false, .has_exponent = false })
#define STR_AND_LEN_F_OF(s, l)                                                 \
    ((str_and_len_tuple_t                                                      \
    ){ .str = (s), .len = (l), .is_float = true, .has_exponent = false })
#define STR_AND_LEN_E_OF(s, l)                                                 \
    ((str_and_len_tuple_t                                                      \
    ){ .str = (s), .len = (l), .is_float = false, .has_exponent = true })
#define STR_AND_LEN_FE_OF(s, l)                                                \
    ((str_and_len_tuple_t                                                      \
    ){ .str = (s), .len = (l), .is_float = true, .has_exponent = true })

#define EXP_LONG_OF(n, e) ((exp_long_t){ .number = (n), .exponent = (e) })
#define EXP_DOUBLE_OF(n, e) ((exp_double_t){ .number = (n), .exponent = (e) })

#define GET_STR_LEN(idx)                                                       \
    while ((c = buff[(idx)++]))                                                \
    {                                                                          \
        if (c == '"' && (idx) > 1 && buff[(idx) - 2])                          \
        {                                                                      \
            break;                                                             \
        }                                                                      \
    }

#define GET_NUM_LEN(idx)                                                       \
    while ((c = buff[(idx)]))                                                  \
    {                                                                          \
        switch (c)                                                             \
        {                                                                      \
        case '+':                                                              \
        case '-':                                                              \
        case '.':                                                              \
        case 'e':                                                              \
        case 'E':                                                              \
        case '0':                                                              \
        case '1':                                                              \
        case '2':                                                              \
        case '3':                                                              \
        case '4':                                                              \
        case '5':                                                              \
        case '6':                                                              \
        case '7':                                                              \
        case '8':                                                              \
        case '9':                                                              \
            ++(idx);                                                           \
            continue;                                                          \
        }                                                                      \
        break;                                                                 \
    }

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \typedef str_and_len_tuple_t
** \brief Used by the parse_number() function to return multiple informations
*/
typedef struct
{
    char *str;
    size_t len;
    bool is_float;
    bool has_exponent;
} str_and_len_tuple_t;

typedef struct
{
    u8 has_exponent; // 0 => false | 1 => true | 2 => error
    union
    {
        exp_long_t long_exp_value;
        i64 long_value;
    };
} long_with_or_without_exponent_t;

typedef struct
{
    u8 has_exponent; // 0 => false | 1 => true | 2 => error
    union
    {
        exp_double_t double_exp_value;
        double double_value;
    };
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
long_with_or_without_exponent_t str_to_long(str_and_len_tuple_t sl);
/**
** \brief Takes sl's char array and transforms it into a double.
**        If the number has an exponent, the exponent is parsed as well and the
**        number is elevated to that exponent
** \param sl A pointer to an str_and_len_tuple_t object
** \returns ERROR_DWOWE in case of error, a struct containing the number with
**          and without exponent, as well as a field indicating whether the
**          number has an exponent
*/
double_with_or_without_exponent_t str_to_double(str_and_len_tuple_t sl);

/***************************************
**              FUNCTIONS             **
***************************************/
/**
** \brief Parses the string starting at 'pos + 1' (first char after the '"')
** \param buff The buffer containing the current json file's contents
** \param idx A pointer to the index in the buffer (this index will be
**            incremented at the end of the function)
** \returns The parsed string, or NULL_STRING in case of error
*/
string_t parse_string(const char *const buff, size_t *idx);

/**
** \brief Reads the buffer from the given pos
** \param buff The buffer containing the current json file's contents
** \param idx A pointer to the index in the buffer (this index will be
**            incremented at the end of the function)
** \returns An str_and_len_tuple_t struct containing the number as a char array,
**          the length of the char array and whether the number is a float and
**          has an exponent, or NULL_STR_AND_LEN_TUPLE if there was an error
*/
str_and_len_tuple_t parse_number(const char *const buff, size_t *idx);

/**
** \param buff The current json file's contents
** \param idx A pointer to the index in the buffer (this index will be
**            incremented at the end of the function)
** \returns 5 if false, 4 if true, 0 otherwise
*/
size_t parse_boolean(const char *const buff, size_t *idx);

/**
** \brief Counts the number of elements in the array that is at the current idx
** \param idx The index of the character just after the '[' that begins the
**            current array
** \returns The number of elements of the current array
*/
size_t get_nb_elts_array(const char *const buff, size_t idx);

/**
** \brief Counts the number of elements in the dict that is at the current idx
** \param buff The buffer containing the object currently being parsed
** \param idx The index of the character just after the '{' that begins the
**            current dict
** \returns The number of elements of the current dict
*/
size_t get_nb_elts_dict(const char *const buff, size_t idx);

#endif // !BASE_JSON_PARSER_H
