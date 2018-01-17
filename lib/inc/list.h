#ifndef LIST_H
#define LIST_H

#include "common.h"

/**
 * token to indicate type of the list node data
 */
typedef enum list_type_token_t {
    DATA_TOKEN_EMPTY   = 0,
    DATA_TOKEN_STRING  = 1,
    DATA_TOKEN_LETTER  = 2,
    DATA_TOKEN_INTEGER = 3
} list_type_token;

/**
 * general purpose node of list
 */
typedef struct list_t {
    void *data;
    list_type_token token;
    struct list_t *ptr;
} list;

/**
 * @brief      create head of string list
 *
 * @return     list head pointer
 */
list *list_create_head(void);

/**
 * @brief      add list entry
 *
 * @param      head  list head
 *
 * @return     new entry pointer
 */
list *list_new_entry(list *head);

/**
 * @brief      write data to the list node
 * NOTE:       allocates/reallocates memory on heap if needed
 * 
 * @param      entry        list entry
 * @param[in]  data         data ptr
 * @param[in]  type_token   data type token
 *
 * @return     error flag
 */
bool list_write(list *entry, const void *data,
    const list_type_token type_token);

/**
 * @brief      delete and free memory of string list
 *
 * @param      head  list head
 */
void list_delete_by_head(list *head);

/**
 * @brief      clear and dealocate node data
 *
 * @param      entry  list entry
 */
void list_node_purge(list *entry);

#endif
