#ifndef JSON_TYPES_H
#define JSON_TYPES_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define T_STR 0
#define T_INT 1
#define T_DOUBLE 2
#define T_BOOL 3
#define T_NULL 4
#define T_ARR 5
#define T_DICT 6
#define T_ERROR 7

#define ERR_FSEEK (1 << 0)
#define ERR_NULL_KEY (1 << 1)
#define ERR_NULL_STR (1 << 2)
#define ERR_NULL_ARR (1 << 3)
#define ERR_NULL_DICT (1 << 4)
#define ERR_ITEM_EXISTS (1 << 5)
#define ERR_MAX_NESTED_ARRAYS_REACHED (1 << 6)
#define ERR_MAX_NESTED_DICTS_REACHED (1 << 7)
#define ERR_NULL_VALUE (1 << 8)
#define ERR_NULL_ITEM (1 << 9)

#ifndef MAX_STR_LEN
#    define MAX_STR_LEN UINT_FAST16_MAX
#endif

#if MAX_STR_LEN <= UINT_FAST16_MAX
typedef uint_fast16_t uint_strlen_t;
#elif MAX_STR_LEN <= UINT_FAST32_MAX
typedef uint_fast32_t uint_strlen_t;
#else
typedef uint_fast64_t uint_strlen_t;
#endif

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct string
{
    char *str;
    uint_strlen_t length;
};

typedef struct string String;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
char strings_equals(struct string s1, struct string s2);

void print_err_bits(uint_fast16_t err);

#endif // !JSON_TYPES_H
