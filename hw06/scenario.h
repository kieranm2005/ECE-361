/**
 * File format (from project sheet):
 *   # comment lines and blank lines are ignored
 *   start <unix_timestamp>          first data line
 *   <tick> <temp> <rh> 0x<switches> one or more lines, ticks strictly increasing
 * Between lines the values hold.
 *
 * At most SCENARIO_MAX_LINES lines, in a fixed array: no malloc.
 */

#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

typedef struct {
    uint32_t tick; // tick number of this line
    float    temp_c; // temperature in Celsius
    float    rh_pct; // relative humidity in percent
    uint32_t switches; // bitmask of switch states
} scenario_line_t;

typedef struct {
    time_t           start; // Unix timestamp of the first data line
    int              nlines; // number of lines in the scenario
    scenario_line_t  lines[SCENARIO_MAX_LINES]; // fixed array
} scenario_t;

/**
 * scenario_load() - parses a scenario file into *s
 *
 * Returns 0 on success. Otherwise returns the 1-based physical line number of
 * the first bad line (counting comments and blanks). On failure *s is not
 * partially populated in a way callers should use; do not read it.
 *
 * Rejected, as errors rather than silently ignored:
 *   - a data line before "start", or a second "start" line
 *   - a line with too few or too many fields
 *   - "switches" without a 0x prefix
 *   - an unparseable number
 *   - a tick that does not strictly increase
 *   - more than SCENARIO_MAX_LINES data lines
 */
int scenario_load(const char *path, scenario_t *s);

/**
 * scenario_at() - the line in force at a tick
 *
 * Values hold between lines, so the line in force is the last line whose tick
 * is <= the argument.
 *
 * returns  0 and fills *out on success; -1 if tick precedes the first line
 *          (no world is defined there)
 */
int scenario_at(const scenario_t *s, uint32_t tick, scenario_line_t *out);

/**
 * scenario_last_tick() - tick of the last line, i.e. how far the replay runs
 * returns the last tick, or 0 if the scenario has no lines
 */
uint32_t scenario_last_tick(const scenario_t *s);

#endif