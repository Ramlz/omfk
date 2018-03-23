#include "utils/putil.h"
#include "kernel/peons.h"
#include "lib/string.h"

#define PUTIL_CMD_STOP    "stop"
#define PUTIL_CMD_RESUME  "resume"
#define PUTIL_CMD_STAT    "stat"

extern peon *peon_curr;
extern peon peon_idler;

static void peon_stat(void) {
    peon_lock();
    {
        peon *current = &peon_idler;
        int cnt = 0;

        printf("________________PEON INFO________________\r");

        printf("IDLER ADDR                   : 0x%X\r", current);
        printf("IDLER STACK BASE             : 0x%X\r",
            current->context.sp_base);
        printf("IDLER CURRENT STACK          : 0x%X\r",
            current->context.sp);
        printf("IDLER STATUS                 : %d\r",
                current->status);

        current = current->next;

        while (current != &peon_idler) {
            printf("\rPEON #%d: %s\r", cnt++, current->name);
            printf("    PEON ADDR                : 0x%X\r",
                current);
            printf("    PEON STACK BASE          : 0x%X\r",
                current->context.sp_base);
            printf("    PEON CURRENT STACK       : 0x%X\r",
                current->context.sp);
            printf("    PEON CURRENT STATUS      : %d\r",
                current->status);
            current = current->next;
        }
    }
    peon_unlock();
}

int putil_cmd_handler(list_iface *args) {
    int size = args->size(args);
    if (size > 3 || size < 2) {
        return -1;
    }

    char *arg = args->get(args, 1);
    if (strcmp(arg, PUTIL_CMD_STOP) == 0) {
        if (size == 3) {
            arg = args->get(args, 2);
            peon_stop_by_name(arg);
            return 0;
        }
    } else if (strcmp(arg, PUTIL_CMD_RESUME) == 0) {
        if (size == 3) {
            arg = args->get(args, 2);
            peon_resume_by_name(arg);
            return 0;
        }
    } else if (strcmp(arg, PUTIL_CMD_STAT) == 0) {
        if (size == 2) {
            peon_stat();
            return 0;
        }
    }

    return -1;
}
