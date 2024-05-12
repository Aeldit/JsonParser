#ifndef GENERIC_LISTS_H
#define GENERIC_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct list_elt
{
    void *value;
    unsigned char type;
};

/**
** \brief Represents a JSON array. It must be initialized with the correct
**        capacity before adding elements to it
*/
struct generic_list
{
    size_t size;
    size_t idx;
    struct list_elt *elts;
};

typedef struct generic_list generic_list_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void append(generic_list_st *l, struct list_elt elt);

#endif // !GENERIC_LISTS_H
