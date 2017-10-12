#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"
#include "terminal.h"

#define HEAP_HDR_SIZE sizeof(heap_meta)
#define CELL_HDR_SIZE sizeof(cell)
#define CELL_START ((cell*) (&HEAP_START + HEAP_HDR_SIZE))
#define CELL_REUSE_MAX_OVERHEAD 2

/**
 * @brief      allocated/freed cell header
 */
typedef struct cell_t {
    /**
     * pointer to next cell
     */
    struct cell_t *ptr;
    /**
     * cell data size in bytes (without header)
     */
    uint16_t size;
    /**
     * inditates if cell is used or freed
     */
    bool     used;
} __attribute__((packed,aligned(1))) cell;

/**
 * @brief      heap header
 */
typedef struct heap_meta_t {
    /**
     * pointer to free memory at the end of heap
     */
    cell *free_cell;
    /**
     * start of list with used/freed cells
     */
    cell *used_start;
} __attribute__((packed,aligned(1))) heap_meta;

/**
 * @brief      general heap initialization
 * NOTE: MUST be called before any heap allocation
 */
void heap_init(void);

void heap_stat(void);

/**
 * @brief      allocated new memory cell
 *
 * @param      size  number of bytes to be allocated
 *
 * @return     pointer to allocated cell data
 */
void *cell_alloc(const uint16_t size);

/**
 * @brief      move allocated memory
 *
 * @param      ptr   allocated cell data
 * @param      size  number of bytes to be allocated at new location
 *
 * @return     pointer to allocated cell data
 */
void *cell_realloc(void* ptr, const uint16_t size);

/**
 * @brief      deallocate cell
 *
 * @param      ptr   allocated cell data
 */
void cell_free(void* ptr);

/**
 * @brief      find best-fit freed cell
 *
 * @param      size  desired size in bytes
 *
 * @return     suitable freed cell
 */
static cell *cell_find_in_used(const uint16_t size);

/**
 * @brief      get last allocated cell
 *
 * @return     pointer to last cell
 */
static cell *cell_last();

/**
 * @brief      delete freed cells at the end of used list
 */
static void cell_drop_free_trailing(void);

/**
 * @brief      delete last freed cell
 */
static void cell_delete_last_free(void);

#endif
