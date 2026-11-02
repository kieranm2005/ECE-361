# ECE 361 HW5 — Combination Lock FSM

Code is **3 1 4**. 

Build with `make`, test with `make test`, run with
`./lock < data/codes.txt`, clean with `make clean`. Bonus: `make bonus`.

## State diagram (Mealy)

```mermaid
stateDiagram-v2
    [*] --> LOCKED

    LOCKED --> GOT1   : E_RIGHT / NONE
    LOCKED --> LOCKED : E_WRONG / BEEP
    LOCKED --> LOCKED : E_CLOSE / NONE
    LOCKED --> LOCKED : E_TIMEOUT / NONE

    GOT1 --> GOT2   : E_RIGHT / NONE
    GOT1 --> LOCKED : E_WRONG / BEEP
    GOT1 --> GOT1   : E_CLOSE / NONE
    GOT1 --> LOCKED : E_TIMEOUT / NONE

    GOT2 --> OPEN   : E_RIGHT / UNLOCK
    GOT2 --> LOCKED : E_WRONG / BEEP
    GOT2 --> GOT2   : E_CLOSE / NONE
    GOT2 --> LOCKED : E_TIMEOUT / NONE

    OPEN --> OPEN   : E_RIGHT, E_WRONG / NONE
    OPEN --> LOCKED : E_CLOSE / LOCK
    OPEN --> LOCKED : E_TIMEOUT / LOCK

## Transition table

| state \ event | E_RIGHT      | E_WRONG      | E_CLOSE      | E_TIMEOUT    |
|---------------|--------------|--------------|--------------|--------------|
| LOCKED        | GOT1 / NONE  | LOCKED / BEEP| LOCKED / NONE| LOCKED / NONE|
| GOT1          | GOT2 / NONE  | LOCKED / BEEP| GOT1 / NONE  | LOCKED / NONE|
| GOT2          | OPEN / UNLOCK| LOCKED / BEEP| GOT2 / NONE  | LOCKED / NONE|
| OPEN          | OPEN / NONE  | OPEN / NONE  | LOCKED / LOCK| LOCKED / LOCK|

`lock_table` is a single 2D array indexed `[state][event]`, with every one of
the 16 entries written as a designated initializer. `lock_step` is a single
`lock_table[s][e]` lookup. `lock_event` maps a character to an event for a
state: digits are `E_RIGHT` only when they are the next expected digit of the
code, `c` is `E_CLOSE`, `t` is `E_TIMEOUT`, anything else is `-1`.