#include "utils/cutil.h"
#include "kernel/memory.h"
#include "lib/string.h"

#define CUTIL_CMD_STAT "stat"

extern heap_meta *heap_hdr;

static void heap_stat(void) {
    printf("__________MEMORY USAGE INFO__________\n");
    printf("HEAP HEADER              : 0x%X\n", heap_hdr);
    printf("NEXT FREE CELL           : 0x%X\n", heap_hdr->free_cell);
    printf("FISRT USED CELL          : 0x%X\n", heap_hdr->used_start);
    printf("HEAP HEADER SIZE         : %d bytes\n", HEAP_HDR_SIZE);
    printf("CELL HEADER SIZE         : %d bytes\n\n", CELL_HDR_SIZE);


    cell *cur_cell = heap_hdr->used_start;
    uint16_t counter_total = 0;
    uint16_t counter_used = 0;

    while (cur_cell) {
        printf("CELL %d:\n", counter_total);
        printf("ADDR     : 0x%X\n", cur_cell);
        printf("MEM ADDR : 0x%X\n", cur_cell + 1);
        printf("MEM END  : 0x%X\n", ((uint8_t*) (cur_cell + 1)) +
            cur_cell->size);
        if (cur_cell->used) {
            printf("USED : yes\n");
            counter_used++;
        } else {
            printf("USED : no\n");
        }
        printf("SIZE : %d bytes\n\n", cur_cell->size);
        cur_cell = cur_cell->ptr;
        counter_total++;
    }

    printf("TOTAL CELLS              : %d\n", counter_total);
    printf("TOTAL USED CELLS         : %d\n", counter_used);
    printf("TOTAL FREED CELLS        : %d\n",
        counter_total - counter_used);
}


int cutil_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size == 2 && strcmp(args->get(args, 1), CUTIL_CMD_STAT) == 0) {
        heap_stat();
        return 0;
    }

    return -1;
}
