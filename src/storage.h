#ifndef STORAGE_H
#define STORAGE_H

#include "json_types.h"

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

typedef struct str_array_link StrArrLink;
typedef struct int_array_link IntArrLink;

// Used for functions that return an element of the array
struct array_link
{
    char type;
    String str;
    int integer;
};

typedef struct array_link ArrayLink;

// Linked list for arrays integers
struct array_linked_list
{
    int size;
    int insert_idx;
    // Stores for each index the type of the value, allowing us to not have to
    // search inside all linked lists
    char indexes_types[10];
    // Pointers to the head of each linked list
    StrArrLink *strings_head;
    IntArrLink *integers_head;
    // Stores the tail of each linked list, to allow faster access
    StrArrLink *strings_tail;
    IntArrLink *integers_tail;
};

typedef struct array_linked_list Array;

Array init_array(int size);
void arr_add_int(Array *ial, int value);
ArrayLink array_get(Array *a, int index);
void destroy_array(Array *ial);

#endif // !STORAGE_H
