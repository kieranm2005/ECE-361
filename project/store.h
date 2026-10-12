/*
 * store.h: ECE 361, Fall 2026, project 19 server room cooling
 *
 * Storage module. Responsible for the device's record of what it has seen:
 * one record_t per tick, holding the reading, the state at that moment, and
 * the status word. Ordered by relative humidity quantized to 1 %RH (key K4),
 * so 45.2 and 45.7 have the same key.
 *
 * Owns the device's history. An opaque type: only store.c knows what is
 * inside. Called by main.c to insert, and by report.c to walk. Does not
 * drive hardware and does not print.
 */
#ifndef STORE_H
#define STORE_H

#include "sensor.h"

/*
 * record_t: what the device remembers about one tick.
 *
 *   reading  -- the timestamped temperature and humidity
 *   state    -- the FSM state at that tick
 *   status   -- the packed status word for that tick
 */
typedef struct {
    reading_t   reading;
    int         state;
    unsigned    status;
} record_t;

/* Opaque handle to the store. Only store.c knows what is inside. */
typedef struct store store_t;

/* Interface to be added in HW3/HW4: store_create, store_insert, store_find,
 * store_walk, store_count, store_height, store_destroy, and record_compare. */

#endif /* STORE_H */