#include "log.h"
#include "clock.h"

static log_entry *log_head = NULL;
static log_entry *log_tmp = NULL;

void log_add(const char* message, ...) {
    char log_buffer[LOG_BUF_SIZE] = "";
    va_list va;
    va_start(va, message);
    peon_lock();
    {
        if (!log_head) {
            log_head = cell_alloc(sizeof(log_entry));
            int ret = vsnprintf(log_buffer, LOG_BUF_SIZE, message, va);
            if (ret < 0) {
                return;
            }
            log_head->message = cell_alloc(strsize(log_buffer));
            strcpy(log_head->message, log_buffer);
            log_head->ptr = NULL;
        } else {
            log_entry *log_tmp = log_head;
            int cnt = 0;
            while (log_tmp->ptr) {
                log_tmp = log_tmp->ptr;
                cnt++;
            }
            log_tmp->ptr = cell_alloc(sizeof(log_entry));
            log_tmp = log_tmp->ptr;
            int ret = vsnprintf(log_buffer, LOG_BUF_SIZE, message, va);
            if (ret < 0) {
                return;
            }
            log_tmp->message = cell_alloc(strsize(log_buffer));
            strcpy(log_tmp->message, log_buffer);
            log_tmp->ptr = NULL;
        }
    }
    peon_unlock();
}

void log_task(void) {
    while (true) {
        clock_dly_secs(1);
        peon_lock();
        {
            if (log_head != NULL && terminal_available()) {
                terminal_info_message("\r[SYSLOG]");
                terminal_output_logs();
                log_clear();
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
