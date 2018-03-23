#ifndef LIST_H
#define LIST_H

#include "common/common.h"

/**
 * interface for general-purpose list container
 */
typedef struct list_iface_t {
    /**
     * @brief      add new entry at the end of list
     *
     * @param      iface      list interface
     * @param      data       entry data ptr
     * @param[in]  data_size  size of data
     *
     * @return     success of adding
     */
    bool (*add)(struct list_iface_t *iface, void *data, uint32_t data_size);
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
    bool (*write)(struct list_iface_t *iface, int index, void *data, uint32_t data_size);
    /**
     * @brief      get list entry by its index
     *
     * @param      iface  list interfase
     * @param[in]  index  entry index
     *
     * @return     list entry ptr
     */
    void* (*get)(struct list_iface_t *iface, int index);
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
    int (*find)(struct list_iface_t *iface, void *data_ptr, uint32_t data_size);
    /**
     * @brief      delete list entry by its index
     *
     * @param      iface  list interface
     * @param[in]  index  entry index
     *
     * @return     success of deletion
     */
    bool (*erase)(struct list_iface_t *iface, int index);
    /**
     * @brief      get list size
     *
     * @param      iface  list interface
     *
     * @return     list entries number
     */
    int (*size)(struct list_iface_t *iface);
    /**
     * @brief      recursively delete list entries and free allocated memory
     *
     * @param      iface  list interface
     */
    void (*destroy)(struct list_iface_t *iface);
} list_iface;

/**
 * @brief      create list interface
 *
 * @return     interface ptr
 */
list_iface *list_create(void);

#endif
