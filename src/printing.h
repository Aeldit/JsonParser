#ifndef PRINTING_H
#define PRINTING_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "lists/json_array.h"
#include "lists/linked_lists.h"

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
void array_print(json_array_st *ja);

void array_print_indent(json_array_st *ja, char indent, char from_list);

void print_json(item_control_st *ctrl);

void print_json_indent(item_control_st *ctrl, char indent);

#endif // !PRINTING_H
