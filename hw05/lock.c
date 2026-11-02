#include "lock.h"

/* lock_table[state][event] -> {next state, action}
 *                          E_RIGHT          E_WRONG           E_CLOSE          E_TIMEOUT   */
const transition_t lock_table[N_STATES][N_EVENTS] = {
[LOCKED] = { {GOT1,   A_NONE}, {LOCKED, A_BEEP}, {LOCKED, A_NONE}, {LOCKED, A_NONE} },
[GOT1]   = { {GOT2,   A_NONE}, {LOCKED, A_BEEP}, {GOT1,   A_NONE}, {LOCKED, A_NONE} },
[GOT2]   = { {OPEN,   A_UNLOCK}, {LOCKED, A_BEEP}, {GOT2,   A_NONE}, {LOCKED, A_NONE} },
[OPEN]   = { {OPEN,   A_NONE}, {OPEN,   A_NONE}, {LOCKED, A_LOCK}, {LOCKED, A_LOCK} },
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