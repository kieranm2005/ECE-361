# Bonus — table checker

`fsm_check` reports four classes of problems and returns the count:
1. missing entries (`A_UNDEFINED`),
2. next state out of range,
3. states unreachable from `start` (BFS),
4. dead-end states (every event self-loops).

## Broken copies and why each is a real bug

**1. Missing `[GOT1][E_CLOSE]` (`A_UNDEFINED`).**
Checker: `missing entry: [GOT1][CLOSE]` (1 problem).
Bug: closing the door while the first digit is entered must change nothing
(stay in `GOT1`). An undefined entry leaves `lock_step` returning a garbage
`next` field — undefined behavior. A user who nudges the door mid-code gets
an unpredictable lock.

**2. `[GOT2][E_WRONG].next = N_STATES`.**
Checker: `next state out of range: [GOT2][WRONG] -> 4` (1 problem).
Bug: a wrong third digit must BEEP and return to `LOCKED`. Indexing
`lock_table[4][...]` next step reads past the table — memory corruption
exactly on the most common path (a typo).

**3. `[GOT1][E_RIGHT].next = GOT1`.**
Checker: `unreachable state: GOT2` and `unreachable state: OPEN` (2 problems).
Bug: the correct code `3 1 4` can never reach `GOT2`, so the lock can never
open — the device is permanently unusable.

(A fourth natural case: every `GOT1` event self-loops →
`dead-end state (no way out): GOT1`; the user is trapped mid-code and the
5-second timer/door close do nothing.)

Each violates a user-visible guarantee — close-mid-code is harmless, wrong
digits reset audibly, and the correct code opens the door — so these are
real lock bugs, caught structurally before anyone types on the keypad.