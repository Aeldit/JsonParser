#ifndef JSON_TYPES_H
#define JSON_TYPES_H

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define TYPE_STR 0
#define TYPE_NUM 1
#define TYPE_OBJ 2
#define TYPE_ARR 3
#define TYPE_BOOL 4
#define TYPE_NULL 5

// Errors
#define TYPE_ERROR 6

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

typedef struct typed_value typed_value_st;

#endif // !JSON_TYPES_H
