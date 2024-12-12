#ifndef JSON_WRITE_H
#define JSON_WRITE_H

#include "json_storage.h"

typedef struct str_link
{
    string_t s;
    char s_needs_free;
    char is_from_str;
    struct str_link *next;
} string_link_t;

typedef struct
{
    string_link_t *head;
    string_link_t *tail;
} string_linked_list_t;

void write_json_to_file(json_t *j, char *file_name);

#endif // !JSON_WRITE_H
