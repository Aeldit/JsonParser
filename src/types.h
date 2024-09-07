#ifndef JSON_TYPES_H
#define JSON_TYPES_H

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_INT 1
#define TYPE_DOUBLE 2
#define TYPE_BOOL 3
#define TYPE_NULL 4
#define TYPE_ARR 5
#define TYPE_DICT 6

// Errors
#define TYPE_ERROR 7

#define SUCCESS 0
#define FAILURE 1

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct typed_value
{
    void *value;
    unsigned char type;
};

/**
** \brief Represents a key:value item
** \param key The key
** \param value Points to the value of the type 'type' inside the corresponding
**        linked list
** \param type The type of the data to which 'value' points
*/
struct item
{
    char *key;
    void *value;
    unsigned char type;
};

typedef struct typed_value typed_value_st;
typedef struct item item_st;

#endif // !JSON_TYPES_H
