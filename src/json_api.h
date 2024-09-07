#ifndef JSON_API_H
#define JSON_API_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "json.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Searches for the value associated with the given key and returns it if
**        it exists.
** \param jd A pointer to the json dict
** \param key The key
** \param key_len The length of the key
*/
typed_value_st get_value(json_dict_st *jd, char *key, size_t key_len);

/**
** \brief Frees all the allocated parts of the 'jd' json dict
*/
void destroy_dict(json_dict_st *jd);

#endif // !JSON_API_H
