#include <stdio.h>
#include <string.h>
#include "../lock.h"
#include "../check.h"

static int failures = 0;

static void run_case(const transition_t t[N_STATES][N_EVENTS],
                     int want, const char *sub, const char *msg)
{
    char buf[4096];
    FILE *f = fmemopen(buf, sizeof buf, "w");
    int got = fsm_check(t, LOCKED, f);
    fclose(f);
    int ok = (got == want) && (sub == NULL || strstr(buf, sub) != NULL);
    if (!ok) {
        failures++;
        printf("FAIL: %s (got %d problems, wanted %d; output:\n%s)\n",
               msg, got, want, buf);
    } else {
        printf("PASS: %s\n", msg);
    }
}

int main(void)
{
    transition_t good[N_STATES][N_EVENTS];
    memcpy(good, lock_table, sizeof good);
    run_case(good, 0, NULL, "good table has no problems");

    transition_t missing[N_STATES][N_EVENTS];
    memcpy(missing, lock_table, sizeof missing);
    missing[GOT1][E_CLOSE].action = A_UNDEFINED;
    run_case(missing, 1, "missing entry: [GOT1][CLOSE]",
             "missing entry detected");

    transition_t oob[N_STATES][N_EVENTS];
    memcpy(oob, lock_table, sizeof oob);
    oob[GOT2][E_WRONG].next = (state_t)N_STATES;
    run_case(oob, 1, "next state out of range",
             "out-of-range next state detected");

    transition_t unreach[N_STATES][N_EVENTS];
    memcpy(unreach, lock_table, sizeof unreach);
    unreach[GOT1][E_RIGHT].next = GOT1;   /* blocks GOT2 and OPEN */
    run_case(unreach, 2, "unreachable state: GOT2",
             "unreachable state detected");

    transition_t dead[N_STATES][N_EVENTS];
    memcpy(dead, lock_table, sizeof dead);
    for (int e = 0; e < N_EVENTS; e++) dead[GOT1][e].next = GOT1;
    run_case(dead, 1, "dead-end state (no way out): GOT1",
             "dead-end state detected");

    printf("\n%d failed\n", failures);
    return failures ? 1 : 0;
}