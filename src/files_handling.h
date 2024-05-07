#ifndef FILES_HANDLING_H
#define FILES_HANDLING_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include <stddef.h>

/*******************************************************************************
**                              DEFINES / MACROS                              **
*******************************************************************************/

#define READ_BUFF_LEN 10

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**
** \brief This is a link of a chained list containing the content of the file
*/
struct file_content
{
    char payload[READ_BUFF_LEN];
    struct file_content *next;
};

/**
** \brief The list looks like this (head is the first element added):
**              NULL <- tail <- elt1 <- elt2 <- head
** \param nb_payloads The number of links in the chained list
*/
struct fc_control
{
    size_t nb_payloads;
    struct file_content *head;
    struct file_content *tail;
};

/*******************************************************************************
**                                 FUNCTIONS                                  **
*******************************************************************************/
/**
** \brief Allocates a file_content structure and returns it
*/
struct file_content *init_file_content(void);

/**
** \brief Allocates a file_content control structure and returns it
*/
struct fc_control *init_fc_control(void);

/**
** \brief Adds the element fc at the tail of fcc
*/
void add_fc_to_fc_control(struct fc_control *fcc, struct file_content *fc);

/**
** \brief Destroys the structure containing the file
*/
void destroy_fc_control(struct fc_control *fcc);

#endif // !FILES_HANDLING_H
