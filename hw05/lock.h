#ifndef LOCK_H
#define LOCK_H

#include <stdio.h>

typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t;
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t;
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t;

typedef struct {
    state_t next;
    action_t action;
} transition_t;

extern const transition_t lock_table[N_STATES][N_EVENTS];

/*Performs one table lookup. Returns the next state, stores the action.*/
state_t lock_step(state_t s, event_t e, action_t *action);

/*Maps char c to an event in state s. Returns -1 for invalid input.
* */
int lock_event(state_t s, char c);

/* Name lookups for states, events, and actions. Returns "?" if out of range.*/
const char *state_name(state_t s);
const char *event_name(event_t e);
const char *action_name(action_t a);

#endif