#include "ptr.h"
#include <stddef.h> // for NULL

void swap_float(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

int minmax(const float *a, int n, float *min, float *max) {
    if (a == NULL || min == NULL || max == NULL || n < 1) {
        return -1;
    }

    const float *p = a; // Initialize pointer to the start of the array
    const float *end = a + n; // Pointer to the end of the array
    float low = *p; // Initialize low to the first element
    float high = *p; // Initialize high to the first element
    p++; // Move pointer to the next element

    while (p < end) {
        float v = *p; // Get the current value pointed to by p
        if (v < low) {
            low = v;
        }
        if (v > high) {
            high = v;
        }
        p++;
    }

    *min = low; // Store the smallest value in *min
    *max = high; // Store the largest value in *max
    return 0;
}

const float *find_above(const float *a, int n, float threshold) {
    if (a == NULL || n < 1) { // Check for null pointer or invalid size
        return NULL;
    }

    const float *p = a; // Initialize pointer to the start of the array
    const float *end = a + n; // Pointer to the end of the array

    while (p < end) { // Iterate through the array
        if (*p > threshold) {
            return p; // Return pointer to the first value greater than threshold
        }
        p++; // Move to the next element
    }
    return NULL; // Return NULL if no value greater than threshold is found
}