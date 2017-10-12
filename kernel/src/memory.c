#include "memory.h"

// heap header
static heap_meta *heap_hdr;

void heap_init(void) {
    // write heap header
    heap_hdr = (heap_meta*) &HEAP_START;
    // set the first cell to be allocated
    heap_hdr->free_cell = CELL_START;
    // clear list of allocated cells
    heap_hdr->used_start = NULL;
}

void *cell_alloc(uint16_t size) {
    // look for suitable freed cell
    cell *cur_cell = cell_find_in_used(size);
    // total cell size to be allocated
    const uint16_t req_space = size + CELL_HDR_SIZE;
    // found suitable freed cell
    if (cur_cell) { 
        cur_cell->used = true;
    // need to allocate at the end
    } else {
        cell *prv_cell = cell_last_used();
        // fisrt allocation
        if (!prv_cell) {
            cur_cell = heap_hdr->free_cell;
            heap_hdr->used_start = cur_cell;
        // commaon allocation at the end of heap
        } else {
            prv_cell->ptr = heap_hdr->free_cell;
            cur_cell = prv_cell->ptr;
        }
        // dirty hack to align pointer to 8-bit
        // TODO: make it looking not like a piece of crap
        char *tmp_cell = (char*) heap_hdr->free_cell + req_space;
        heap_hdr->free_cell = (cell*) tmp_cell;
        // setup new cell header
        cur_cell->ptr = NULL;
        cur_cell->used = true;
        cur_cell->size = size;
    }
    // returning pointer to the end of cell header
    return (cur_cell + CELL_HDR_SIZE);
}

void *cell_realloc(void* ptr, uint16_t size) {
    // get cell header
    cell *cur_cell = (ptr - CELL_HDR_SIZE);
    // returning if reallocating fewer memory
    if (cur_cell->size > size) {
        return NULL;
    }
    // allocating new cell
    uint8_t *new_data = cell_alloc(size);
    // copying data to new location 
    for (uint8_t i = 0; i < size; ++i) {
        *(new_data + i) = *((uint8_t*) ptr + i);
    }
    // free old cell
    cell_free(ptr);
    return new_data;
}

void cell_free(void* ptr) {
    // get cell header
    cell *cur_cell = (ptr - CELL_HDR_SIZE);
    // mark cell as not used
    cur_cell->used = false;
}

static cell *cell_find_in_used(uint16_t size) {
    cell *cur_cell = heap_hdr->used_start;
    // returning if nothing has been allocated yet
    if (!cur_cell) {
        return NULL;
    }
    cell *best_cell = NULL;
    // going throught list of cells
    while (cur_cell->ptr) {
        // suitable not busy cell found
        if (!cur_cell->used && cur_cell->size >= size) {
            // first suitable cell found
            if (!best_cell) {
                best_cell = cur_cell;
            // better cell found
            } else if (cur_cell->size < best_cell->size) {
                best_cell = cur_cell;
            }
        }
        cur_cell = cur_cell->ptr;
    }
    // found suitable freed cell
    if (best_cell) {
        return best_cell;
    }
    return NULL;
}

static cell *cell_last_used() {
    cell *cur_cell = heap_hdr->used_start;
    // returning if nothing has been allocated yet
    if (!cur_cell) {
        return NULL;
    }
    // going throught list of cells
    while (cur_cell->ptr) {
        cur_cell = cur_cell->ptr;
    }
    // last used cell header
    return cur_cell;
}