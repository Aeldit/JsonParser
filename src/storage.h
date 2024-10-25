#ifndef STORAGE_H
#define STORAGE_H

#include "json_types.h"

typedef struct array_linked_list Array;

struct str_array_link
{
    int index; // Index of the element in the array
    String value;
    struct str_array_link *next;
};

struct int_array_link
{
    int index; // Index of the element in the array
    int value;
    struct int_array_link *next;
};

struct double_array_link
{
    int index; // Index of the element in the array
    double value;
    struct double_array_link *next;
};

struct bool_array_link
{
    int index; // Index of the element in the array
    char value;
    struct bool_array_link *next;
};

struct null_array_link
{
    int index; // Index of the element in the array
    struct null_array_link *next;
};

struct arr_array_link
{
    int index; // Index of the element in the array
    Array *value;
    struct arr_array_link *next;
};

/*struct dict_array_link
{
    int index; // Index of the element in the array
    int value;
    struct int_array_link *next;
};*/

typedef struct str_array_link StrArrLink;
typedef struct int_array_link IntArrLink;
typedef struct double_array_link DoubleArrLink;
typedef struct bool_array_link BoolArrLink;
typedef struct null_array_link NullArrLink;
typedef struct arr_array_link ArrArrLink;

// Used for functions that return an element of the array
struct array_link
{
    char type;
    String strv;
    int integerv;
    double doublev;
    char boolv;
    Array *arrayv;
};

typedef struct array_link ArrayLink;

// Linked list for arrays integers
struct array_linked_list
{
    int size;
    int insert_idx;

    // Stores for each index the type of the value, allowing us to not have to
    // search inside all linked lists
    char *indexes_types;

    // Pointers to the head of each linked list
    StrArrLink *strings_head;
    IntArrLink *integers_head;
    DoubleArrLink *doubles_head;
    BoolArrLink *booleans_head;
    NullArrLink *nulls_head;
    ArrArrLink *arrays_head;

    // Stores the tail of each linked list, to allow faster access
    StrArrLink *strings_tail;
    IntArrLink *integers_tail;
    DoubleArrLink *doubles_tail;
    BoolArrLink *booleans_tail;
    NullArrLink *nulls_tail;
    ArrArrLink *arrays_tail;
};

/**
** \brief The size of the returned array is 0, it means that an error occured
**        when allocating the indexes_types array
*/
Array init_array(int size);

void arr_add_str(Array *a, String value);
void arr_add_int(Array *a, int value);
void arr_add_double(Array *a, double value);
void arr_add_bool(Array *a, char value);
void arr_add_null(Array *a);
void arr_add_arr(Array *a, Array *value);

ArrayLink array_get(Array *a, int index);
void destroy_array(Array *a);

#endif // !STORAGE_H
