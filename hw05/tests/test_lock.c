#include <stdio.h>
#include <string.h>
#include "../lock.h"

static int failures = 0;
static int passes = 0;

#define CHECK(cond, msg) do { \
    if (cond) { passes++; printf("PASS: %s\n", msg); } \
    else { failures++; printf("FAIL: %s\n", msg); } \
} while (0)

/* Run a sequence of chars from LOCKED; return final state, record last action. */
static state_t run(const char *seq, action_t *last_action)
{
    state_t s = LOCKED;
    action_t a = A_NONE;
    for (const char *p = seq; *p; p++) {
        if (*p == ' ') continue; // Skip space characters in the sequence
        int e = lock_event(s, *p); // Determine the event based on the current state and input character
        s = lock_step(s, (event_t)e, &a); // Update the state based on the event and record the action
    }
    if (last_action) *last_action = a; // Record the last action performed
    return s;
}

static void test_table_entries(void)
{
    const transition_t exp[N_STATES][N_EVENTS] = {
        [LOCKED] = { [E_RIGHT]={GOT1,A_NONE}, [E_WRONG]={LOCKED,A_BEEP},
                     [E_CLOSE]={LOCKED,A_NONE}, [E_TIMEOUT]={LOCKED,A_NONE} },
        [GOT1]   = { [E_RIGHT]={GOT2,A_NONE}, [E_WRONG]={LOCKED,A_BEEP},
                     [E_CLOSE]={GOT1,A_NONE}, [E_TIMEOUT]={LOCKED,A_NONE} },
        [GOT2]   = { [E_RIGHT]={OPEN,A_UNLOCK}, [E_WRONG]={LOCKED,A_BEEP},
                     [E_CLOSE]={GOT2,A_NONE}, [E_TIMEOUT]={LOCKED,A_NONE} },
        [OPEN]   = { [E_RIGHT]={OPEN,A_NONE}, [E_WRONG]={OPEN,A_NONE},
                     [E_CLOSE]={LOCKED,A_LOCK}, [E_TIMEOUT]={LOCKED,A_LOCK} },
    };
    for (int s = 0; s < N_STATES; s++) { // Iterate over all states
        for (int e = 0; e < N_EVENTS; e++) { // Iterate over all events for the current state
            action_t a;
            state_t n = lock_step((state_t)s, (event_t)e, &a); // Compute the next state and action
            char msg[128];
            snprintf(msg, sizeof msg, "table[%s][%s] = %s/%s",
                     state_name((state_t)s), event_name((event_t)e),
                     state_name(exp[s][e].next), action_name(exp[s][e].action));
            CHECK(n == exp[s][e].next && a == exp[s][e].action, msg); // Verify the computed next state and action against the expected values
        }
    }
}

static void test_lock_event(void)
{
    CHECK(lock_event(LOCKED, '3') == E_RIGHT, "LOCKED '3' RIGHT");
    CHECK(lock_event(LOCKED, '1') == E_WRONG, "LOCKED '1' WRONG");
    CHECK(lock_event(GOT1,   '1') == E_RIGHT, "GOT1 '1' RIGHT");
    CHECK(lock_event(GOT1,   '4') == E_WRONG, "GOT1 '4' WRONG");
    CHECK(lock_event(GOT2,   '4') == E_RIGHT, "GOT2 '4' RIGHT");
    CHECK(lock_event(GOT2,   '3') == E_WRONG, "GOT2 '3' WRONG");
    CHECK(lock_event(OPEN,   '3') == E_WRONG, "OPEN '3' WRONG (no next digit)");
    CHECK(lock_event(OPEN,   '4') == E_WRONG, "OPEN '4' WRONG (no next digit)");
    CHECK(lock_event(LOCKED, 'c') == E_CLOSE,   "LOCKED 'c' CLOSE");
    CHECK(lock_event(GOT1,   't') == E_TIMEOUT, "GOT1 't' TIMEOUT");
    CHECK(lock_event(LOCKED, 'x') == -1,        "LOCKED 'x' invalid");
    CHECK(lock_event(OPEN,   '\n') == -1,       "OPEN newline invalid");
}

static void test_sequences(void)
{
    action_t a;
    CHECK(run("314", &a) == OPEN && a == A_UNLOCK, "3 1 4 opens");
    CHECK(run("3314", &a) == LOCKED && a == A_BEEP, "3 3 1 4 does not open (beeps)");
    CHECK(run("31t4", &a) == LOCKED, "3 1 t 4 does not open (timeout resets)");
    CHECK(run("314c", &a) == LOCKED && a == A_LOCK, "3 1 4 c locks again");
}

static void test_names(void)
{
    CHECK(strcmp(state_name(LOCKED), "LOCKED") == 0, "state_name(LOCKED)");
    CHECK(strcmp(event_name(E_TIMEOUT), "TIMEOUT") == 0, "event_name(TIMEOUT)");
    CHECK(strcmp(action_name(A_UNLOCK), "UNLOCK") == 0, "action_name(UNLOCK)");
    CHECK(strcmp(state_name((state_t)99), "?") == 0, "state_name out of range");
    CHECK(strcmp(action_name((action_t)-1), "?") == 0, "action_name out of range");
}

int main(void)
{
    test_table_entries();
    test_lock_event();
    test_sequences();
    test_names();
    printf("\n%d passed, %d failed\n", passes, failures);
    return failures ? 1 : 0;
}