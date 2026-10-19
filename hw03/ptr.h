#ifndef PTR_H
#define PTR_H

/*
* Exchanges *a and *b.
 */
void swap_float(float *a, float *b);

/*
* Stores the smallest and larges values from a[0..n-1] in *min and *max, and returns 0.
* If n < 1, or any pointer is null, returns -1.
 */
int minmax(const float *a, int n, float *min, float *max);

const float *find_above(const float *a, int n, float threshold);

void reverse(float *a, int n);
#endif