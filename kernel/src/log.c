#include "log.h"
#include "clock.h"

static log_entry *log_head = NULL;
static log_entry *log_tmp = NULL;

void log_add(const char* message) {
    peon_lock();
    {
        if (!log_head) {
            log_head = cell_alloc(sizeof(log_entry));
            log_head->message = cell_alloc(strsize(message));
            log_head->ptr = NULL;
            strcpy(log_head->message, message);
        } else {
            log_entry *log_tmp = log_head;
            int cnt = 0;
            while (log_tmp->ptr) {
                log_tmp = log_tmp->ptr;
                cnt++;
            }
            log_tmp->ptr = cell_alloc(sizeof(log_entry));
            log_tmp = log_tmp->ptr;
            log_tmp->ptr = NULL;
            log_tmp->message = cell_alloc(strsize(message));
            strcpy(log_tmp->message, message);
        }
    }
    peon_unlock();
}

void log_task(void) {
    while (1) {
        clock_dly_secs(1);
        peon_lock();
        {
            if (log_head != NULL && terminal_available()) {
                terminal_info_message("[SYSLOG]");
                terminal_output_logs();
                terminal_clear_logs();
                terminal_new_cmd();
            }
        }
        peon_unlock();
    }
}

/**
 * @brief      delete log list node
 *
 * @param      entry  the log entry
 */
static void log_delete_entry(log_entry *entry) {
    if (entry->ptr) {
        log_delete_entry(entry->ptr);
    }
    cell_free(entry->message);
    cell_free(entry);
}

void log_clear(void) {
    peon_lock();
    {   
        if (log_head) {
            log_delete_entry(log_head);
            log_head = NULL;
        }
    }
    peon_unlock();
}

void log_start_read(void) {
    log_tmp = log_head;
}

char* log_get(void) {
    {
        char *msg_ptr;
        if (log_tmp) {
            msg_ptr = log_tmp->message;
            log_tmp = log_tmp->ptr;
            return msg_ptr;
        } else {
            log_tmp = log_head;
            return NULL;
        }
    }
}
