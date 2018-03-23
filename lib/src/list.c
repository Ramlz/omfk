#include "lib/list.h"
#include "kernel/memory.h"
#include "lib/string.h"

/**
 * general-purpose node of list
 */
typedef struct node_t {
    void *data;
    struct node_t *ptr;
} node;

/**
 * internal list container context
 */
typedef struct list_ctx_t {
    list_iface iface;
    node *list_head;
} list_ctx;

/**
 * @brief      list deletion subroitine
 *
 * NOTE: shouldn't be called independently
 *
 * @param      entry  list entry
 */
static void list_delete_entry_unsafe(node *entry) {
    if (entry->ptr) {
        list_delete_entry_unsafe(entry->ptr);
    }
    cell_free(entry->data);
    cell_free(entry);
}

/**
 * @brief      recursively delete list entries and free allocated memory
 *
 * @param      iface  list interface
 */
static void list_destroy(list_iface *iface) {
    list_ctx *ctx = (list_ctx*) iface;

    if (ctx->list_head) {
        list_delete_entry_unsafe(ctx->list_head);
    }

    cell_free(ctx);
}

/**
 * @brief      get list size
 *
 * @param      iface  list interface
 *
 * @return     list entries number
 */
static int list_size(list_iface *iface) {
    list_ctx *ctx = (list_ctx*) iface;
    if (!ctx->list_head) {
        return 0;
    }

    int i = 0;

    node *tmp = ctx->list_head;
    while (tmp) {
        tmp = tmp->ptr;
        i++;
    }

    return i;
}

/**
 * @brief      delete list entry by its index
 *
 * @param      iface  list interface
 * @param[in]  index  entry index
 *
 * @return     success of deletion
 */
static bool list_erase(list_iface *iface, int index) {
    list_ctx *ctx = (list_ctx*) iface;
    if (!ctx->list_head) {
        return false;
    }

    int i = 0;

    node *tmp = ctx->list_head;
    while (tmp->ptr && i < index - 1) {
        tmp = tmp->ptr;
        i++;
    }

    if (i == index - 1) {
        node *erased = tmp->ptr;
        if (erased) {
            tmp->ptr = erased->ptr;
        } else {
            tmp->ptr = NULL;
        }
        cell_free(erased->data);
        cell_free(erased);
        return true;
    }

    return false;
}

/**
 * @brief      find list entry by its content
 *
 * NOTE:       looks for suitable entry by bare memory comparation
 *
 * @param      iface      list interface
 * @param      data       data ptr to compare
 * @param[in]  data_size  size of data to compare
 *
 * @return     entry index if found, otherwise -1
 */
static int list_find(list_iface *iface, void *data, uint32_t data_size) {
    list_ctx *ctx = (list_ctx*) iface;

    int i = 0;

    node *tmp = ctx->list_head;
    while (tmp) {
        if (memcmp(data, tmp->data, data_size) == 0) {
            return i;
        }

        tmp = tmp->ptr;
        i++;
    }

    return -1;
}

/**
 * @brief      get list entry by its index
 *
 * @param      iface  list interfase
 * @param[in]  index  entry index
 *
 * @return     list entry ptr
 */
static void *list_get(list_iface *iface, int index) {
    list_ctx *ctx = (list_ctx*) iface;
    if (!ctx->list_head) {
        return false;
    }

    int i = 0;

    node *tmp = ctx->list_head;
    while (tmp->ptr && i < index) {
        tmp = tmp->ptr;
        i++;
    }

    if (i == index) {
        return tmp->data;
    }

    return NULL;
}

/**
 * @brief      override list entry at given index
 *
 * NOTE:       deallocates previous entry content
 *
 * @param      iface      list interface
 * @param[in]  index      entry index
 * @param      data       data to be written
 * @param[in]  data_size  date size
 *
 * @return     success of write
 */
static bool list_write(list_iface *iface, int index, void *data,
    uint32_t data_size) {
    list_ctx *ctx = (list_ctx*) iface;
    if (!ctx->list_head) {
        return false;
    }

    void *new_data = cell_alloc(data_size);
    if (!new_data) {
        return false;
    }

    int i = 0;

    node *tmp = ctx->list_head;
    while (tmp->ptr && i < index) {
        tmp = tmp->ptr;
        i++;
    }

    if (i == index) {
        cell_free(tmp->data);
        tmp->data = new_data;
        memcpy(tmp->data, data, data_size);
    } else {
        cell_free(new_data);
        return false;
    }

    return true;
}

/**
 * @brief      add new entry at the end of list
 *
 * @param      iface      list interface
 * @param      data       entry data ptr
 * @param[in]  data_size  size of data
 *
 * @return     success of adding
 */
static bool list_add(list_iface *iface, void *data, uint32_t data_size) {
    list_ctx *ctx = (list_ctx*) iface;
    node *entry = cell_alloc(sizeof(node));
    if (!entry) {
        return false;
    }
    entry->ptr = NULL;

    node *tmp = ctx->list_head;
    if (!tmp) {
        goto apply;
    }

    while (tmp->ptr) {
        tmp = tmp->ptr;
    }

    apply:

    entry->data = cell_alloc(data_size);
    if (!entry->data) {
        cell_free(entry);
        return false;
    }
    memcpy(entry->data, data, data_size);

    if (!ctx->list_head) {
        ctx->list_head = entry;
    } else {
        tmp->ptr = entry;
    }

    return true;
}

list_iface *list_create(void) {
    list_ctx *ctx = cell_alloc(sizeof(list_ctx));
    if (!ctx) {
        return NULL;
    }

    list_iface *iface = &ctx->iface;
    iface->add = list_add;
    iface->write = list_write;
    iface->get = list_get;
    iface->find = list_find;
    iface->erase = list_erase;
    iface->size = list_size;
    iface->destroy = list_destroy;

    ctx->list_head = NULL;

    return iface;
}
