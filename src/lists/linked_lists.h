#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "../json.h"
#include "../types.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***********************************************************
**                       LILFECYCLE                       **
***********************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct pair *append_pair(json_dict_st *jd, struct pair *value);

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
const char *append_key(json_dict_st *jd, const char *value);

void destroy_key_control(key_control_st *ctrl);

/***************************************
**                STR                 **
***************************************/
char *append_str(json_dict_st *jd, char *value);

void destroy_str_control(str_control_st *sc);

/***************************************
**                NUM                 **
***************************************/
long *append_num(json_dict_st *jd, long value);

void destroy_num_control(num_control_st *ctrl);

/***************************************
**             JSON DICT              **
***************************************/
json_dict_st *append_json_dict(json_dict_st *jd, json_dict_st *value);

void destroy_json_dict_control(json_dict_control_st *ctrl);

/***************************************
**               ARRAY                **
***************************************/
json_array_st *append_array(json_dict_st *jd, json_array_st *value);

void destroy_array_control(list_control_st *ctrl);

/***************************************
**               BOOL                 **
***************************************/
char *append_bool(json_dict_st *jd, char value);

void destroy_bool_control(bool_control_st *ctrl);

/***********************************************************
**                         UTILS                          **
***********************************************************/
void print_array(json_array_st *l, char indent, char from_list);

char key_exists(json_dict_st *jd, const char *key);

typed_value_st get_value(json_dict_st *jd, const char *key);

#endif // !LINKED_LISTS_H
