#include "memory.h"

/**
 * heap header
 */
static heap_meta *heap_hdr;

/**
 * @brief      find best-fit freed cell
 *
 * @param      size  desired size in bytes
 *
 * @return     suitable freed cell
 */
static cell *cell_find_in_used(const uint16_t size) {
    cell *cur_cell = heap_hdr->used_start;
    cell *best_cell = NULL;
    //! going throught list of cells
    while (cur_cell) {
        //! suitable not busy cell found
        if (!cur_cell->used && cur_cell->size >= size &&
            size * CELL_REUSE_MAX_OVERHEAD >= cur_cell->size) {
            //! first suitable cell found
            if (!best_cell) {
                best_cell = cur_cell;
            //! better cell found
            } else if (cur_cell->size < best_cell->size) {
                best_cell = cur_cell;
            }
        }
        cur_cell = cur_cell->ptr;
    }
    //! found suitable freed cell or NULL
    return best_cell ? best_cell : NULL;
}

/**
 * @brief      get last allocated cell
 *
 * @return     pointer to last cell
 */
static cell *cell_last(void) {
    cell *cur_cell = heap_hdr->used_start;
    //! returning if nothing has been allocated yet
    if (!cur_cell) {
        return NULL;
    }
    //! going throught list of cells
    while (cur_cell->ptr) {
        cur_cell = cur_cell->ptr;
    }
    //! last used cell header
    return cur_cell;
}

/**
 * @brief      delete last freed cell
 */
static void cell_delete_last_free(void) {
    cell *curr_cell = heap_hdr->used_start;
    cell *prev_cell;
    //!reinitialize heap if the first allocated cell marked as free
    if (!curr_cell->ptr && !curr_cell->used) {
        heap_init();
        return;
    } else {
        while (curr_cell->ptr) {
            prev_cell = curr_cell;
            curr_cell = curr_cell->ptr;
        }
        if (!curr_cell->used) {
            prev_cell->ptr = NULL;
            heap_hdr->free_cell = curr_cell;
        }
    }
}

/**
 * @brief      delete freed cells at the end of used list
 */
static void cell_drop_free_trailing(void) {
    while (!(cell_last()->used) && heap_hdr->used_start) {
        cell_delete_last_free();
    }
}

void heap_init(void) {
    //! write heap header
    heap_hdr = (heap_meta*) &HEAP_START;
    //! set the first cell to be allocated
    heap_hdr->free_cell = CELL_START;
    //! clear list of allocated cells
    heap_hdr->used_start = NULL;
}

void heap_stat(void) {
    terminal_info_message("__________MEMORY USAGE INFO__________");
    terminal_printf("HEAP HEADER              : 0x%X  ", heap_hdr);
    terminal_printf("NEXT FREE CELL           : 0x%X  ", heap_hdr->free_cell);
    terminal_printf("FISRT USED CELL          : 0x%X\n", heap_hdr->used_start);
    terminal_printf("HEAP HEADER SIZE         : %d bytes  ", HEAP_HDR_SIZE);
    terminal_printf("CELL HEADER SIZE         : %d bytes\n", CELL_HDR_SIZE);


    cell *cur_cell = heap_hdr->used_start;
    uint16_t counter_total = 0;
    uint16_t counter_used = 0;

    while (cur_cell) {
        terminal_printf("CELL %d:", counter_total);
        terminal_printf("ADDR     : 0x%X", cur_cell);
        terminal_printf("MEM ADDR : 0x%X", cur_cell + 1);
        terminal_printf("MEM END  : 0x%X", ((uint8_t*) (cur_cell + 1)) +
            cur_cell->size);
        if (cur_cell->used) {
            terminal_printf("USED : yes");
            counter_used++;
        } else {
            terminal_printf("USED : no");
        }
        terminal_printf("SIZE : %d bytes", cur_cell->size);
        cur_cell = cur_cell->ptr;
        terminal_printf("");
        counter_total++;
    }

    terminal_printf("TOTAL CELLS              : %d", counter_total);
    terminal_printf("TOTAL USED CELLS         : %d", counter_used);
    terminal_printf("TOTAL FREED CELLS        : %d",
        counter_total - counter_used);
}

void *cell_alloc(const uint16_t size) {
    //! look for suitable freed cell
    cell *cur_cell = cell_find_in_used(size);
    //! total cell size to be allocated
    const uint16_t req_space = ALIGN_MEM(size) + CELL_HDR_SIZE;
    //! found suitable freed cell
    if (cur_cell) { 
        cur_cell->used = true;
    //! need to allocate at the end
    } else {
        cell *prv_cell = cell_last();
        //! fisrt allocation
        if (!prv_cell) {
            cur_cell = heap_hdr->free_cell;
            heap_hdr->used_start = cur_cell;
        //! common allocation at the end of heap
        } else {
            //! no memory
            if ((cell*) ((uint8_t*) heap_hdr->free_cell + req_space) >
                (cell*) &STACK_END) {
                return NULL;
            }
            prv_cell->ptr = heap_hdr->free_cell;
            cur_cell = prv_cell->ptr;
        }
        heap_hdr->free_cell =
            (cell*) ((uint8_t*) heap_hdr->free_cell + req_space);
        //! setup new cell header
        cur_cell->ptr = NULL;
        cur_cell->used = true;
        cur_cell->size = req_space - CELL_HDR_SIZE;
    }
    //! returning pointer to the end of cell header
    return cur_cell + 1;
}

void *cell_realloc(void* ptr, const uint16_t size) {
    //! get cell header
    cell *cur_cell = (cell*) ptr;
    ptr--;
    //! returning if reallocating fewer memory
    if (cur_cell->size > size) {
        return NULL;
    }
    //! allocating new cell
    uint8_t *new_data = cell_alloc(size);

    if (new_data) {
        //! copying data to new location 
        for (uint8_t i = 0; i < size; ++i) {
            *(new_data + i) = *((uint8_t*) ptr + i);
        }
        //! free old cell
        cell_free(ptr);
    }
    return new_data;
}

void cell_free(void* ptr) {
    if (!ptr) {
        return;
    }
    //! get cell header
    cell *cur_cell = (cell*) ptr - 1;
    //! mark cell as not used
    cur_cell->used = false;
    //! delete freed cell at the end of list
    cell_drop_free_trailing();
}
