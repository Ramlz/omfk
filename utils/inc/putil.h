#ifndef PUTIL_H
#define PUTIL_H

#include "common.h"

/**
 * @brief      pauses thread by it's name
 *
 * @param      name  thread's name
 *
 * @return     error code
 */
void peon_stop_by_name(char *name);

/**
 * @brief      resumes thread by it's name
 *
 * @param      name  thread's name
 *
 * @return     error code
 */
void peon_resume_by_name(char *name);

/**
 * @brief      locks current thread
 */
void peon_lock(void);

/**
 * @brief      unlocks current thread
 */
void peon_unlock(void);

#endif
