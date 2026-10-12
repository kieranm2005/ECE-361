#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#define HISTOGRAM_NUM_BINS 20
#define HISTOGRAM_BIN_WIDTH 5.0f

/*
 * Takes an array of n temperature values and prints a histogram using
 * HISTOGRAM_NUM_BINS bins of HISTOGRAM_BIN_WIDTH degrees. Returns nothing.
 * Assumes a points to at least n values and n is nonnegative; values below
 * zero and above the final bin are clamped to the first and last bins.
 */
void histogram_print(const float a[], int n);
#endif