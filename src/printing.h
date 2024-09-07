#ifndef PRINTING_H
#define PRINTING_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "linked_list.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void print_array(json_array_st *ja);

void print_array_indent(json_array_st *ja, char indent, char from_list);

void print_json(json_dict_st *jd);

void print_json_indent(json_dict_st *jd, char indent, char from_dict);

#endif // !PRINTING_H
