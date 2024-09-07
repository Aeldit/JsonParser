#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stdint.h>

#include "types.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define BASE_ARRAY_LEN 16

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct link_value
{
    typed_value_st elts[BASE_ARRAY_LEN];
    struct link_value *next;
};

struct linked_list_value
{
    uint64_t size;
    uint64_t insert_idx;
    uint64_t nb_deletion;
    struct link_value *head;
    struct link_value *tail;

    // TODO: Implement
    /*void defragment(){}*/
};

struct link_item
{
    item_st elts[BASE_ARRAY_LEN];
    struct link_item *next;
};

struct linked_list_item
{
    uint64_t size;
    uint64_t insert_idx;
    uint64_t nb_deletion;
    struct link_item *head;
    struct link_item *tail;

    // TODO: Implement
    /*void defragment(){}*/
};

typedef struct linked_list_value json_array_st;
typedef struct linked_list_item json_dict_st;

/**
** \brief Iterates over the arrays in the linked list and increments the
**        number of elements encountered when the current element is not
**        nullptr
** \param index The index of the element we want. This is NOT the index
**              if the lists were put together in one, but the index of
**              non-TYPE_ERROR elements
**              Ex: with the array [0, 4, 3, TYPE_ERROR, 5, TYPE_ERROR, 8],
**                  the element of index 4 is '8'
** \returns The element at the given index if it exists, NULL otherwise
*/
typed_value_st *getValue(json_array_st *ja, uint64_t index);
item_st *getItem(json_dict_st *jd, uint64_t index);

void addValue(json_array_st *ja, typed_value_st value);
void addItem(json_dict_st *jd, item_st value);

void removevalue(json_array_st *ja, uint64_t index);
void removeItem(json_dict_st *jd, uint64_t index);

void destroy_json_array(json_array_st *ja);
void destroy_json_dict(json_dict_st *jd);

#endif // !LINKED_LIST_H
