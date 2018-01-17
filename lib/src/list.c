#include "list.h"
#include "memory.h"
#include "string.h"

list *list_create_head(void) {
    list *head = cell_alloc(sizeof(list));
    head->ptr = NULL;
    head->token = DATA_TOKEN_EMPTY;
    head->data = NULL;
    return head;
}

list *list_new_entry(list *head) {
    list *node = head;
    while (node->ptr) {
        node = node->ptr;
    }

    node->ptr = cell_alloc(sizeof(list));
    node = node->ptr;
    node->ptr = NULL;
    node->token = DATA_TOKEN_EMPTY;
    node->data = NULL;
    return node;
}

bool list_write(list *entry, const void *data,
    const list_type_token type_token) {
    void *new_data_ptr = NULL;

    switch (type_token) {
        case DATA_TOKEN_EMPTY:
            return false;
        case DATA_TOKEN_STRING:
            new_data_ptr = cell_alloc(strsize(data));
            break;
        case DATA_TOKEN_LETTER:
            new_data_ptr = cell_alloc(sizeof(char));
            break;
        case DATA_TOKEN_INTEGER:
            new_data_ptr = cell_alloc(sizeof(int));
            break;
        default:
            return false;
    }

    if (new_data_ptr) {
        if (entry->token  != DATA_TOKEN_EMPTY) {
            list_node_purge(entry);
        }

        entry->data = new_data_ptr;
        entry->token = type_token;

        switch (type_token) {
            case DATA_TOKEN_EMPTY:
                return false;
            case DATA_TOKEN_STRING:
                strcpy(entry->data, data);
                return true;
            case DATA_TOKEN_LETTER:
                *(char*) entry->data = *(char*) data;
                return true;
            case DATA_TOKEN_INTEGER:
                *(int*) entry->data = *(int*) data;
                return true;
        }
    }
    return false;
}

void list_node_purge(list *entry) {
    entry->token = DATA_TOKEN_EMPTY;
    cell_free(entry->data);
}

/**
 * @brief      list deletion subroitine
 * NOTE: shouldn't be called independently
 *
 * @param      entry  list entry
 */
static void list_delete_entry_unsafe(list *entry) {
    if (entry->ptr) {
        list_delete_entry_unsafe(entry->ptr);
    }
    cell_free(entry->data);
    cell_free(entry);
}

void list_delete_by_head(list *head) {
    if (head) {
        list_delete_entry_unsafe(head);
    }
}
