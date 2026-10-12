/*
 * aux.h: ECE 361, Fall 2026, project 19 server room cooling
 *
 * Auxiliary module. Responsible for the circular buffer of the last
 * N = 60 readings (second structure S3). When the buffer is full, a new
 * reading overwrites the oldest. The buffer keeps the device's recent
 * history in order, newest first, so the report and the trace can ask for
 * the i-th newest reading without walking the whole store.
 *
 * Owns the recent-history ring. An opaque type. Called by main.c to add, and
 * by report.c to read. Does not drive hardware and does not print.
 */
#ifndef AUX_H
#define AUX_H

#include "sensor.h"

/* N is a constant in one place: 60 readings. */
#define AUX_CAPACITY 60

/* Opaque handle to the circular buffer. Only aux.c knows what is inside. */
typedef struct aux aux_t;

/* Interface to be added in HW7: aux_create, aux_add, aux_get, aux_count,
 * aux_is_full, aux_destroy. */

#endif /* AUX_H */