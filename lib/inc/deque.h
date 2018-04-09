#ifndef DEQUE_H
#define DEQUE_H

#include "common/common.h"


/**
 * interface for general-purpose deque container
 */
typedef struct deque_iface_t {
    /**
     * @brief      add new entry at the start of deque
     *
     * @param      iface      deque interface
     * @param      data       entry data ptr
     * @param[in]  data_size  size of data
     *
     * @return     success of adding
     */
    bool (*push_front)(struct deque_iface_t *iface, void *data, uint32_t data_size);
    /**
     * @brief      add new entry at the end of deque
     *
     * @param      iface      deque interface
     * @param      data       entry data ptr
     * @param[in]  data_size  size of data
     *
     * @return     success of adding
     */
    bool (*push_back)(struct deque_iface_t *iface, void *data, uint32_t data_size);
    /**
     * @brief      get entry from the head of deque and delete it
     *
     * @param      iface      deque interface
     *
     * @return     entry
     */
    void* (*pop_front)(struct deque_iface_t *iface);
    /**
     * @brief      get entry from the end of deque and delete it
     *
     * @param      iface      deque interface
     *
     * @return     entry
     */
    void* (*pop_back)(struct deque_iface_t *iface);
    /**
     * @brief      get entry from the head of deque
     *
     * @param      iface      deque interface
     *
     * @return     entry
     */
    void* (*peek_front)(struct deque_iface_t *iface);
    /**
     * @brief      get entry from the tail of deque
     *
     * @param      iface      deque interface
     *
     * @return     entry
     */
    void* (*peek_back)(struct deque_iface_t *iface);
    /**
     * @brief      get entry by its index from head
     *
     * @param      iface      deque interface
     * @param      index      position from the head
     *
     * @return     entry
     */
    void* (*get_front)(struct deque_iface_t *iface, int index);
    /**
     * @brief      get entry by its index from tail
     *
     * @param      iface      deque interface
     * @param      index      position from the tail
     *
     * @return     entry
     */
    void* (*get_back)(struct deque_iface_t *iface, int index);
    /**
     * @brief      find index of element from the head of deque
     *
     * @param      iface      deque interface
     * @param      data       data for search
     * @param      data_size  size of data
     *
     * @return     entry
     */
    int (*find_front)(struct deque_iface_t *iface, void *data, uint32_t data_size);
    /**
     * @brief      find index of element from the tail of deque
     *
     * @param      iface      deque interface
     * @param      data       data for search
     * @param      data_size  size of data
     *
     * @return     entry
     */
    int (*find_back)(struct deque_iface_t *iface, void *data, uint32_t data_size);
    /**
     * @brief      get deque size
     *
     * @param      iface  deque interface
     *
     * @return     deque entries number
     */
    int (*size)(struct deque_iface_t *iface);
    /**
     * @brief      delete deque entries and free allocated memory
     *
     * @param      iface  deque interface
     */
    void (*destroy)(struct deque_iface_t *iface);

} deque_iface;


deque_iface *deque_create(void);

#endif
