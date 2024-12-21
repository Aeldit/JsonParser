#ifndef RW_JSON_WRITE_H
#define RW_JSON_WRITE_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "rw_json_storage.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
typedef struct rw_str_link
{
    string_t s;
    char s_needs_free;
    char is_from_str;
    struct rw_str_link *next;
} rw_string_link_t;

typedef struct
{
    rw_string_link_t *head;
    rw_string_link_t *tail;
} rw_string_linked_list_t;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void write_rw_json_to_file(rw_json_t *j, char *file_name);

#endif // !RW_JSON_WRITE_H
