#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

#include "../api.h"
#include "../json.h"

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/
#define CHAR_BUFF_LEN 64

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
struct linked_list_char
{
    char buff[CHAR_BUFF_LEN];
    struct linked_list_char *next;
};

// Represents a string of undefined length
struct linked_list_char_ctrl
{
    size_t len;
    size_t idx;
    struct linked_list_char *head;
};

typedef struct linked_list_char_ctrl ll_char_ctrl_st;

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/***********************************************************
**                       LILFECYCLE                       **
***********************************************************/
/***************************************
**               PAIR                 **
***************************************/
struct item *append_item(json_dict_st *jd, struct item *value);

void destroy_item_control(item_control_st *ctrl);

/***************************************
**                KEY                 **
***************************************/
/**
** \brief  Appends the given key to the keys array
** \return The index of the added key + 1 (0 is the error code), so we have to
**         make sure later to substract 1 to the result of this function
*/
char *append_key(json_dict_st *jd, char *value);

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
char key_exists(json_dict_st *jd, char *key);

/***********************************************************
**                    LINKED LISTS STR                    **
***********************************************************/
ll_char_ctrl_st *init_ll(void);

void add_char_to_ll(ll_char_ctrl_st *llcc, char c);

/**
** \brief Creates an allocated string and frees the linked list links
** \return An allocated string created from all the caracters found in the
**         linked list
*/
char *get_final_string(ll_char_ctrl_st *llcc);

void destroy_llcc(ll_char_ctrl_st *llcc);

#endif // !LINKED_LISTS_H
