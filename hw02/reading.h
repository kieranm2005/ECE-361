#ifndef READING_H
#define READING_H

#include <stdio.h>

#define READING_MAX_READINGS 1000
#define READING_LINE_LEN 128

/*
 * Takes an input stream and arrays with room for READING_MAX_READINGS entries.
 * Reads valid "tick temperature humidity" records, writes the number stored to
 * count, and writes the number of malformed records skipped to skipped.
 * Assumes in, count, skipped, and all output arrays are non-NULL and writable.
 */
void reading_read(FILE *in, int ticks[], float temps[], float hums[], int *count, int *skipped);

#endif
