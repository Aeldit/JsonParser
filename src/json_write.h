#ifndef JSON_WRITE_H
#define JSON_WRITE_H

#include "json_storage.h"

typedef struct str_link
{
    String s;
    char s_needs_free;
    struct str_link *next;
} StringLink;

typedef struct
{
    StringLink *head;
    StringLink *tail;
} StringLinkedList;

void write_json_to_file(JSON *j, char *file_name);

#endif // !JSON_WRITE_H
