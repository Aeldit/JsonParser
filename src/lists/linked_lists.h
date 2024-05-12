#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "../json.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct pair *append_pair(pair_control_st *ctrl, struct pair *value);

void print_json(pair_control_st *ctrl);

void destroy_pair_control(pair_control_st *ctrl);

/***************************************
**                KEY                 **
***************************************/
/**
** \brief  Appends the given key to the keys array
** \return The index of the added key + 1 (0 is the error code), so we have to
**         make sure later to substract 1 to the result of this function
*/
const char *append_key(key_control_st *ctrl, const char *value);

void destroy_key_control(key_control_st *ctrl);

/***************************************
**                STR                 **
***************************************/
char *append_str(str_control_st *ctrl, char *value);

void destroy_str_control(str_control_st *sc);

/***************************************
**                NUM                 **
***************************************/
long *append_num(num_control_st *ctrl, long value);

void destroy_num_control(num_control_st *ctrl);

/***************************************
**             JSON DICT              **
***************************************/
json_dict_st *append_json_dict(json_dict_control_st *ctrl, json_dict_st *value);

void destroy_json_dict_control(json_dict_control_st *ctrl);

/***************************************
**               ARRAY                **
***************************************/
generic_list_st *append_list(list_control_st *ctrl, generic_list_st *value);

void destroy_list_control(list_control_st *ctrl);

/***************************************
**               BOOL                 **
***************************************/
char *append_bool(bool_control_st *ctrl, char value);

void destroy_bool_control(bool_control_st *ctrl);

#endif // !LINKED_LISTS_H
