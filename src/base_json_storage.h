#ifndef BASE_JSON_STORAGE_H
#define BASE_JSON_STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define STRING_OF(s, l)                                                        \
    ((string_t){ .str = (s), .len = (l), .needs_freeing = true })
#define STRING_NOFREE_OF(s, l)                                                 \
    ((string_t){ .str = (s), .len = (l), .needs_freeing = false })
#define NULL_STRING ((string_t){ .str = 0, .len = 0, .needs_freeing = false })

/*******************************************************************************
**                                  TYPEDEFS                                  **
*******************************************************************************/
#ifdef LEAST
typedef int_least8_t i8;
typedef uint_least8_t u8;
typedef int_least16_t i16;
typedef uint_least16_t u16;
typedef int_least32_t i32;
typedef uint_least32_t u32;
typedef int_least64_t i64;
typedef uint_least64_t u64;
#else
typedef int_fast8_t i8;
typedef uint_fast8_t u8;
typedef int_fast16_t i16;
typedef uint_fast16_t u16;
typedef int_fast32_t i32;
typedef uint_fast32_t u32;
typedef int_fast64_t i64;
typedef uint_fast64_t u64;
#endif // !LEAST

/*******************************************************************************
**                                    ENUMS                                   **
*******************************************************************************/
enum elt_type
{
    T_ERROR,
    T_STR,
    T_LONG,
    T_DOUBLE,
    T_EXP_LONG,
    T_EXP_DOUBLE,
    T_BOOL,
    T_NULL,
    T_ARR,
    T_DICT,
};

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct
{
    char *str;
    size_t len;
    bool needs_freeing;
} string_t;

typedef struct
{
    i64 number;
    i64 exponent;
} exp_long_t;

typedef struct
{
    double number;
    i64 exponent;
} exp_double_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \returns A string_t representation of the given char array
*/
string_t string_of(char *s);
/**
** \returns A string_t representation of the given char array but specifies that
**          the string must no be freed (usually used when manually writing the
**          string in the code)
*/
string_t string_nofree_of(char *s);

bool strings_equals(string_t s1, string_t s2);
bool exp_long_equals(exp_long_t a, exp_long_t b);
bool exp_double_equals(exp_double_t a, exp_double_t b);

/**
** \brief Frees the memory allocated to the string when necessary
*/
void destroy_string(string_t s);

#endif // !BASE_JSON_STORAGE_H
