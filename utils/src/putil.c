#include "putil.h"
#include "peons.h"

extern peon *peon_curr;
extern peon peon_idler;

void peon_lock(void) {
    peon_curr->status = LOCKED;
}

void peon_unlock(void) {
    peon_curr->status = RUNNING;
}

void peon_stop_by_name(char *name) {
    peon_lock();
    {
        peon *current = &peon_idler;

        do {
            current = current->next;
            if (strncmp(current->name, name, strlen(name)) == 0) {
                break;
            }
        } while (current != &peon_idler);

        if (current != &peon_idler && current != peon_curr) {
            current->status = STOPPED;
        } else {
            log_add("failed to stop thread: %s", name);
        }
    }
    peon_unlock();
}

void peon_resume_by_name(char *name) {
    peon_lock();
    {
        peon *current = &peon_idler;

        do {
            current = current->next;
            if (strncmp(current->name, name, strlen(name)) == 0) {
                break;
            }
        } while (current->next != &peon_idler);

        if (current != &peon_idler && current != peon_curr) {
            current->status = READY;
        } else {
            log_add("failed to resume thread: %s", name);
        }
    }
    peon_unlock();
}
