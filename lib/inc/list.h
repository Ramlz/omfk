#ifndef LIST_H
#define LIST_H

#include "common.h"

/**
 * general purpose node of string list
 */
typedef struct list_str {
    char *str;
    struct list_str *ptr;
} list_str;

/**
 * @brief      create head of string list
 *
 * @return     list head pointer
 */
list_str *list_create_head(void);

/**
 * @brief      add list entry
 *
 * @param      head  list head
 *
 * @return     new entry pointer
 */
list_str *list_str_new_entry(list_str *head);

/**
 * @brief      write string to the list node
 * NOTE:       allocates/reallocates memory on heap if needed
 * 
 * @param      entry  list entry
 * @param[in]  str    string
 *
 * @return     error flag
 */
bool list_str_write(list_str *entry, const char *str);

/**
 * @brief      delete and free memory of string list
 *
 * @param      head  list head
 */
void list_str_delete_by_head(list_str *head);

#endif
