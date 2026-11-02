#include <stdio.h>
#include <string.h>
#include "check.h"

int fsm_check(const transition_t table[N_STATES][N_EVENTS],
              state_t start, FILE *out) // Check the FSM for various problems
{
    int problems = 0;
    int reached[N_STATES] = {0};

    /* 1. missing entries */
    for (int s = 0; s < N_STATES; s++) { // Check each state for missing entries
        for (int e = 0; e < N_EVENTS; e++) { // Check each event for the current state
            if (table[s][e].action == A_UNDEFINED) { // Missing entry
                fprintf(out, "missing entry: [%s][%s]\n",
                        state_name((state_t)s), event_name((event_t)e));
                problems++;
            }
        }
    }

    /* 2. next state out of range */
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            int n = (int)table[s][e].next; // Get the next state for the current state and event
            if (n < 0 || n >= N_STATES) { // Check if the next state is out of range
                fprintf(out, "next state out of range: [%s][%s] -> %d\n",
                        state_name((state_t)s), event_name((event_t)e), n);
                problems++;
            }
        }
    }

    /* 3. unreachable states (BFS over in-range transitions) */
    if ((int)start >= 0 && start < N_STATES) {
        state_t queue[N_STATES]; // Queue for BFS
        int head = 0, tail = 0; // Queue indices
        reached[start] = 1; // Mark the start state as reached
        queue[tail++] = start; // Add the start state to the queue
        while (head < tail) { // Process each state in the queue
            state_t s = queue[head++]; // Dequeue the current state
            for (int e = 0; e < N_EVENTS; e++) { // Check each event for the current state
                int n = (int)table[s][e].next; // Get the next state for the current state and event
                if (n >= 0 && n < N_STATES && !reached[n]) { // If the next state is valid and not yet reached
                    reached[n] = 1; // Mark the next state as reached
                    queue[tail++] = (state_t)n; // Add the next state to the queue
                }
            }
        }
        for (int s = 0; s < N_STATES; s++) { // Check each state for reachability
            if (!reached[s]) { // If the state was not reached
                fprintf(out, "unreachable state: %s\n", state_name((state_t)s));
                problems++;
            }
        }
    }

    /* 4. states with no way out: every event self-loops (or is invalid) */
    for (int s = 0; s < N_STATES; s++) {
        int stuck = 1; // Assume the state is stuck initially
        for (int e = 0; e < N_EVENTS; e++) {
            if ((int)table[s][e].next != s) { stuck = 0; break; } // If any event leads to a different state, the state is not stuck
        }
        if (stuck) {
            fprintf(out, "dead-end state (no way out): %s\n",
                    state_name((state_t)s));
            problems++;
        }
    }

    return problems;
}