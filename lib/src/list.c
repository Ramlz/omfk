#include "list.h"
#include "memory.h"
#include "string.h"

list_str *list_create_head(void) {
    list_str *head = cell_alloc(sizeof(list_str));
    head->ptr = NULL;
    head->str = NULL;
    return head;
}

list_str *list_str_new_entry(list_str *head) {
    list_str *node = head;
    while (node->ptr) {
        node = node->ptr;
    }

    node->ptr = cell_alloc(sizeof(list_str));
    node = node->ptr;
    node->ptr = NULL;
    node->str = NULL;
    return node;
}

bool list_str_write(list_str *entry, const char *str) {
    if (!entry->str) {
        //! allocate memory for string
        entry->str = cell_alloc(strsize(str));
    } else {
        //! string already allocated
        char *new_str = cell_realloc(entry->str, strsize(str));
        if (new_str) {
            entry->str = new_str;
        }
    }
    if (entry->str) {
        strcpy(entry->str, str);
        return true;
    } else {
        return false;
    }
}

/**
 * @brief      list deletion subroitine
 * NOTE: shouldn't be called independently
 *
 * @param      entry  list entry
 */
static void list_str_delete_entry(list_str *entry) {
    if (entry->ptr) {
        list_str_delete_entry(entry->ptr);
    }
    cell_free(entry->str);
    cell_free(entry);
}

void list_str_delete_by_head(list_str *head) {
    if (head) {
        list_str_delete_entry(head);
    }
}
