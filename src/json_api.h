#ifndef JSON_API_H
#define JSON_API_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "linked_list.h"
#include "parser.h"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct json
{
    char is_array;
    storage_st *storage;
    json_array_st *ja;
    json_dict_st *jd;
};

typedef struct json json_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
json_st parse(char *file);

/**
** \brief Searches for the value at the given index and returns it if it exists.
** \param ja A pointer to the json array
** \param index The index to search for
*/
typed_value_st get_value_at(json_array_st *ja, uint64_t index);

/**
** \brief Searches for the value associated with the given key and returns it if
**        it exists.
** \param jd A pointer to the json dict
** \param key The key
** \param key_len The length of the key
*/
item_st get_item(json_dict_st *jd, char *key, uint64_t key_len);

/**
** \brief Frees all the allocated parts of the json object
*/
void destroy_json(json_st *j);

#endif // !JSON_API_H
