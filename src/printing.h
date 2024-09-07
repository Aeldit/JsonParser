#ifndef PRINTING_H
#define PRINTING_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "lists/linked_lists.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void print_array(json_array_st *ja);

void print_array_indent(json_array_st *ja, char indent, char from_list);

void print_json(item_control_st *ctrl);

void print_json_indent(item_control_st *ctrl, char indent);

#endif // !PRINTING_H
