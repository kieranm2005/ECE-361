#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include "lock.h"

/* Returns number of problems found. Prints one line per problem to out. */
int fsm_check(const transition_t table[N_STATES][N_EVENTS],
              state_t start, FILE *out);

#endif