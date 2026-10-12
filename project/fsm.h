/*
 * fsm.h: ECE 361, Fall 2026, project 19 server room cooling
 *
 * Finite-state-machine module. Responsible for the four states (NORMAL,
 * ELEVATED, CRITICAL, FAILOVER), the events the device reacts to, the
 * transition table, and the outputs each state drives on the three LEDs and
 * the RGB LED.
 *
 * Owns the control logic of the server room cooling monitor. Consumes events
 * built from a reading_t and the switch register; returns the next state and
 * the action. Reads nothing from hardware directly, and does not print.
 */
#ifndef FSM_H
#define FSM_H

/* The four states, in the order the sheet lists them. */
typedef enum {
    STATE_NORMAL = 0,
    STATE_ELEVATED,
    STATE_CRITICAL,
    STATE_FAILOVER
} fsm_state_t;

/* Events to be added in HW5, together with the transition table. */

#endif /* FSM_H */