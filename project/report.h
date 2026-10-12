/*
 * report.h: ECE 361, Fall 2026, project 19 server room cooling
 *
 * Report module. Responsible for the device's record product (report Q6):
 * the first and the last tick at which the humidity was outside the safe
 * band of 20 to 80 %RH, and the total number of such readings. If the
 * condition never held, it reports nothing and says so.
 *
 * Owns everything the device prints about its history. Reads the store
 * through store.h only. Does not drive hardware.
 */
#ifndef REPORT_H
#define REPORT_H

#include "store.h"

/*
 * report_summary_t: the result of walking the store for the Q6 condition.
 *
 *   first_tick  -- timestamp of the first out-of-band humidity, or
 *                  (time_t) -1 if the condition never held
 *   last_tick   -- timestamp of the last out-of-band humidity, or
 *                  (time_t) -1 if the condition never held
 *   count       -- number of readings whose humidity was outside 20 to 80
 */
typedef struct {
    time_t first_tick;
    time_t last_tick;
    int    count;
} report_summary_t;

/* Interface to be added in HW9: report_first_last, report_print. */

#endif /* REPORT_H */