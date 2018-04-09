#include "lib/deque.h"
#include "kernel/memory.h"


/**
 * general-purpose node of deque
 */
typedef struct node_t {
    void *data;
    struct node_t *prev;
    struct node_t *next;
} node;

/**
 * internal deque container context
 */
typedef struct deque_ctx_t {
    deque_iface iface;
    node *deque_head;
    node *deque_tail;
} deque_ctx;

/**
 * @brief      get deque size
 *
 * @param      iface  deque interface
 *
 * @return     deque entries number
 */
static int deque_size(deque_iface *iface) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_head) {
        return 0;
    }

    int i = 0;
    node *tmp = ctx->deque_head;

    while(tmp) {
        tmp = tmp->prev;
        i++;
    }

    return i;
}

/**
 * @brief      add new entry at the end of deque
 *
 * @param      iface      deque interface
 * @param      data       entry data ptr
 * @param[in]  data_size  size of data
 *
 * @return     success of adding
 */
static bool push_back(deque_iface *iface, void *data, uint32_t data_size) {
    deque_ctx *ctx = (deque_ctx*) iface;

    node *entry = cell_alloc(sizeof(node));
    if(!entry) {
        return false;
    }

    entry->data = cell_alloc(data_size);
    if(!entry->data) {
        return false;
    }

    entry->prev = NULL;
    entry->next = NULL;
    memcpy(entry->data, data, data_size);

    if(!ctx->deque_tail) {
        ctx->deque_tail = entry;
        ctx->deque_head = entry;
    }else {
        entry->next = ctx->deque_tail;
        ctx->deque_tail->prev = entry;
        ctx->deque_tail = entry;
    }

    return true;
};


/**
 * @brief      add new entry at the start of deque
 *
 * @param      iface      deque interface
 * @param      data       entry data ptr
 * @param[in]  data_size  size of data
 *
 * @return     success of adding
 */
static bool push_front(deque_iface *iface, void *data, uint32_t data_size) {
    deque_ctx *ctx = (deque_ctx*) iface;

    node *entry = cell_alloc(sizeof(node));
    if(!entry) {
        return false;
    }

    entry->data = cell_alloc(data_size);
    if(!entry->data) {
        return false;
    }


    entry->next = NULL;
    entry->prev = NULL;
    memcpy(entry->data, data, data_size);


    if(!ctx->deque_head) {
        ctx->deque_head = entry;
        ctx->deque_tail = entry;
    }else {
        entry->prev = ctx->deque_head;
        ctx->deque_head->next = entry;
        ctx->deque_head = entry;
    }

    return true;
}

/**
 * @brief      get entry from the tail of deque and delete it
 *
 * @param      iface      deque interface
 *
 * @return     entry
 */
static void *pop_back(deque_iface *iface) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_tail) {
        return NULL;
    }

    void *data = ctx->deque_tail->data;
    if(ctx->deque_tail->next) {
        ctx->deque_tail = ctx->deque_tail->next;
        ctx->deque_tail->prev = NULL;
    }else {
        ctx->deque_tail = NULL;
        ctx->deque_head = NULL;
    }

    return data;
}

/**
 * @brief      get entry from the head of deque and delete it
 *
 * @param      iface      deque interface
 *
 * @return     entry
 */
static void *pop_front(deque_iface *iface) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_head) {
        return NULL;
    }

    void *data = ctx->deque_head->data;

    if(ctx->deque_head->prev) {
        ctx->deque_head = ctx->deque_head->prev;
        ctx->deque_head->next = NULL;
    }else {
        ctx->deque_head = NULL;
        ctx->deque_tail = NULL;
    }

    return data;
}

/**
 * @brief      get entry from the head of deque
 *
 * @param      iface      deque interface
 *
 * @return     entry
 */
static void *peek_front(deque_iface *iface) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_head) {
        return NULL;
    }

    return ctx->deque_head->data;
}

/**
 * @brief      get entry from the tail
 *
 * @param      iface      deque interface
 *
 * @return     entry
 */
static void *peek_back(deque_iface *iface) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_tail) {
        return NULL;
    }

    return ctx->deque_tail->data;
}

/**
 * @brief      get entry by its index from head
 *
 * @param      iface      deque interface
 * @param      index      position from the head
 *
 * @return     entry
 */
static void *get_front(deque_iface *iface, int index) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_head){
        return NULL;
    }

    node *tmp = ctx->deque_head;
    int i = 0;
    while(tmp){
        if(i == index){
            return tmp->data;
        }
        i++;
        tmp = tmp->prev;
    }

    return NULL;

}

/**
 * @brief      get entry from by its index from tail
 *
 * @param      iface      deque interface
 * @param      index      position from the tail
 *
 * @return     entry
 */
static void *get_back(deque_iface *iface, int index) {
    deque_ctx *ctx = (deque_ctx*) iface;
    if(!ctx->deque_tail){
        return NULL;
    }

    node *tmp = ctx->deque_tail;
    int i = 0;
    while(tmp) {
        if(i == index) {
            return tmp->data;
        }
        i++;
        tmp = tmp->next;
    }

    return NULL;
}

/**
 * @brief      find index of element from the head of deque
 *
 * @param      iface      deque interface
 * @param      data       data for search
 * @param      data_size  size of data
 *
 * @return     entry
 */
static int find_front(deque_iface *iface, void *data, uint32_t data_size) {
    deque_ctx *ctx = (deque_ctx*) iface;
    int i = 0;
    node *tmp = ctx->deque_head;

    while(tmp) {
        if(memcmp(data, tmp->data, data_size) == 0) {
            return i;
        }
        i++;
        tmp = tmp->prev;
    }

    return -1;
}

/**
 * @brief      find index of element from the tail of deque
 *
 * @param      iface      deque interface
 * @param      data       data for search
 * @param      data_size  size of data
 *
 * @return     entry
 */
static int find_back(deque_iface *iface, void *data, uint32_t data_size) {
    deque_ctx *ctx = (deque_ctx*) iface;
    int i = 0;
    node *tmp = ctx->deque_tail;

    while(tmp) {
        if(memcmp(data, tmp->data, data_size) == 0) {
                return i;
        }
        i++;
        tmp = tmp->next;
    }

    return -1;
}

  /**
* @brief      delete deque entries and free allocated memory
*
* @param      iface  deque interface
*/
static void destroy(deque_iface *iface){
    deque_ctx *ctx = (deque_ctx*) iface;

    node *tmp = ctx->deque_head;
    while(tmp) {
        cell_free(tmp->data);
        tmp = tmp->prev;
        cell_free(tmp->next);
    }
}

deque_iface *deque_create(void) {
    deque_ctx *ctx = cell_alloc(sizeof(deque_ctx));
    if(!ctx) {
        return NULL;
    }

    deque_iface *iface = &ctx->iface;

    iface->size = deque_size;

    iface->push_front = push_front;
    iface->push_back = push_back;

    iface->pop_front = pop_front;
    iface->pop_back = pop_back;

    iface->peek_front = peek_front;
    iface->peek_back = peek_back;

    iface->get_front = get_front;
    iface->get_back = get_back;

    iface->find_front = find_front;
    iface->find_back = find_back;

    iface->destroy = destroy;

    ctx->deque_head = NULL;
    ctx->deque_tail = NULL;
    return iface;
}
