#include "lock.h"

/* lock_table[state][event] -> {next state, action}
 *                          E_RIGHT          E_WRONG           E_CLOSE          E_TIMEOUT   */
const transition_t lock_table[N_STATES][N_EVENTS] = {
/* LOCKED */ [LOCKED] = {
                 [E_RIGHT]   = { GOT1,   A_NONE   },
                 [E_WRONG]   = { LOCKED, A_BEEP   },
                 [E_CLOSE]   = { LOCKED, A_NONE   },
                 [E_TIMEOUT] = { LOCKED, A_NONE   } },
/* GOT1   */ [GOT1]   = {
                 [E_RIGHT]   = { GOT2,   A_NONE   },
                 [E_WRONG]   = { LOCKED, A_BEEP   },
                 [E_CLOSE]   = { GOT1,   A_NONE   },
                 [E_TIMEOUT] = { LOCKED, A_NONE   } },
/* GOT2   */ [GOT2]   = {
                 [E_RIGHT]   = { OPEN,   A_UNLOCK },
                 [E_WRONG]   = { LOCKED, A_BEEP   },
                 [E_CLOSE]   = { GOT2,   A_NONE   },
                 [E_TIMEOUT] = { LOCKED, A_NONE   } },
/* OPEN   */ [OPEN]   = {
                 [E_RIGHT]   = { OPEN,   A_NONE   },
                 [E_WRONG]   = { OPEN,   A_NONE   },
                 [E_CLOSE]   = { LOCKED, A_LOCK   },
                 [E_TIMEOUT] = { LOCKED, A_LOCK   } },
};

/* One lookup, no if/switch on s*/
state_t lock_step(state_t s, event_t e, action_t *action)
{
    transition_t t = lock_table[s][e];
    if (action)
        *action = t.action; // Store the action to be performed
    return t.next; // Return the next state
}

/* Next expected digit for each state, or '\0' if none. */
static char next_digit(state_t s)
{
    switch (s) {
    case LOCKED: return '3'; // First digit of the combination
    case GOT1:   return '1'; // Second digit of the combination
    case GOT2:   return '4'; // Third digit of the combination
    default:     return '\0'; 
    }
}

int lock_event(state_t s, char c)
{
    if (c >= '0' && c <= '9') { // Check if the character is a digit
        char want = next_digit(s); // Get the next expected digit for the current state
        return (want != '\0' && c == want) ? E_RIGHT : E_WRONG; // Return the appropriate event based on the digit
    }
    if (c == 'c') return E_CLOSE; // Return the close event
    if (c == 't') return E_TIMEOUT; // Return the timeout event
    return -1; // Return -1 for invalid input
}

static const char *const state_names[N_STATES] = {
    [LOCKED] = "LOCKED", [GOT1] = "GOT1", [GOT2] = "GOT2", [OPEN] = "OPEN",
};
static const char *const event_names[N_EVENTS] = {
    [E_RIGHT] = "RIGHT", [E_WRONG] = "WRONG",
    [E_CLOSE] = "CLOSE", [E_TIMEOUT] = "TIMEOUT",
};
static const char *const action_names[N_ACTIONS] = {
    [A_UNDEFINED] = "UNDEFINED", [A_NONE] = "NONE",
    [A_UNLOCK] = "UNLOCK", [A_LOCK] = "LOCK", [A_BEEP] = "BEEP",
};

const char *state_name(state_t s)
{
    if ((int)s < 0 || s >= N_STATES) return "?";
    return state_names[s];
}
const char *event_name(event_t e)
{
    if ((int)e < 0 || e >= N_EVENTS) return "?";
    return event_names[e];
}
const char *action_name(action_t a)
{
    if ((int)a < 0 || a >= N_ACTIONS) return "?";
    return action_names[a];
}