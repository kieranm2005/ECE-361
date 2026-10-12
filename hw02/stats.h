#ifndef STATS_H
#define STATS_H

/*
 * Takes an array of n floating-point values and returns its smallest value.
 * Assumes a points to at least n elements and n is greater than zero.
 */
float stats_min(const float a[], int n);

/*
 * Takes an array of n floating-point values and returns its largest value.
 * Assumes a points to at least n elements and n is greater than zero.
 */
float stats_max(const float a[], int n);

/*
 * Takes an array of n floating-point values and returns their arithmetic mean.
 * Assumes a points to at least n elements and n is greater than zero.
 */
float stats_mean(const float a[], int n);

/*
 * Takes an array, its length, a threshold, and an output pointer. Returns the
 * length of the longest consecutive run of values above threshold and writes
 * that run's starting index to start_out. Assumes a has n elements, n is
 * nonnegative, and start_out is non-NULL.
 */
int stats_longest_run_above(const float a[], int n, float threshold, int *start_out);
#endif