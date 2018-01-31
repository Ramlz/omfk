#include "stat.h"
#include "memory.h"
#include "peons.h"

extern peon *peon_curr;
extern peon peon_idler;
extern heap_meta *heap_hdr;

void peon_stat(void) {
    peon_lock();
    {
        peon *current = &peon_idler;
        int cnt = 0;

        terminal_info_message("________________PEON INFO________________");

        terminal_printf("IDLER ADDR                   : 0x%X\r", current);
        terminal_printf("IDLER STACK BASE             : 0x%X\r",
            current->context.sp_base);
        terminal_printf("IDLER CURRENT STACK          : 0x%X\r",
            current->context.sp);
        terminal_printf("IDLER STATUS                 : %d\r",
                current->status);

        current = current->next;

        while (current != &peon_idler) {
            terminal_printf("\rPEON #%d: %s\r", cnt++, current->name);
            terminal_printf("    PEON ADDR                : 0x%X\r",
                current);
            terminal_printf("    PEON STACK BASE          : 0x%X\r",
                current->context.sp_base);
            terminal_printf("    PEON CURRENT STACK       : 0x%X\r",
                current->context.sp);
            terminal_printf("    PEON CURRENT STATUS      : %d\r",
                current->status);
            current = current->next;
        }
    }
    peon_unlock();
}

void heap_stat(void) {
    terminal_info_message("__________MEMORY USAGE INFO__________");
    terminal_printf("HEAP HEADER              : 0x%X\r", heap_hdr);
    terminal_printf("NEXT FREE CELL           : 0x%X\r", heap_hdr->free_cell);
    terminal_printf("FISRT USED CELL          : 0x%X\r", heap_hdr->used_start);
    terminal_printf("HEAP HEADER SIZE         : %d bytes\r", HEAP_HDR_SIZE);
    terminal_printf("CELL HEADER SIZE         : %d bytes\r\r", CELL_HDR_SIZE);


    cell *cur_cell = heap_hdr->used_start;
    uint16_t counter_total = 0;
    uint16_t counter_used = 0;

    while (cur_cell) {
        terminal_printf("CELL %d:\r", counter_total);
        terminal_printf("ADDR     : 0x%X\r", cur_cell);
        terminal_printf("MEM ADDR : 0x%X\r", cur_cell + 1);
        terminal_printf("MEM END  : 0x%X\r", ((uint8_t*) (cur_cell + 1)) +
            cur_cell->size);
        if (cur_cell->used) {
            terminal_printf("USED : yes\r");
            counter_used++;
        } else {
            terminal_printf("USED : no\r");
        }
        terminal_printf("SIZE : %d bytes\r\r", cur_cell->size);
        cur_cell = cur_cell->ptr;
        counter_total++;
    }

    terminal_printf("TOTAL CELLS              : %d\r", counter_total);
    terminal_printf("TOTAL USED CELLS         : %d\r", counter_used);
    terminal_printf("TOTAL FREED CELLS        : %d\r",
        counter_total - counter_used);
}
