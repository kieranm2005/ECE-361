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

    const float *start = a; // Initialize pointer to the start of the array
    const float *end = a + n; // Pointer to the end of the array
    float low = *start; // Initialize low to the first element
    float high = *start; // Initialize high to the first element
    start++; // Move pointer to the next element

    while (start < end) {
        float v = *start; // Get the current value pointed to by start
        if (v < low) {
            low = v;
        }
        if (v > high) {
            high = v;
        }
        start++;
    }

    *min = low; // Store the smallest value in *min
    *max = high; // Store the largest value in *max
    return 0;
}

const float *find_above(const float *a, int n, float threshold) {
    if (a == NULL || n < 1) { // Check for null pointer or invalid size
        return NULL;
    }

    const float *start = a; // Initialize pointer to the start of the array
    const float *end = a + n; // Pointer to the end of the array

    while (start < end) { // Iterate through the array
        if (*start > threshold) {
            return start; // Return pointer to the first value greater than threshold
        }
        start++; // Move to the next element
    }
    return NULL; // Return NULL if no value greater than threshold is found
}

void reverse(float *a, int n) {
    if (a == NULL || n < 2) {
        return; // Do nothing if the array is NULL or has less than 2 elements
    }

    float *start = a; // Pointer to the start of the array
    float *end = a + n - 1; // Pointer to the end of the array

    while (start < end) {
        swap_float(start, end); // Swap the values at start and end
        start++; // Move start pointer forward
        end--; // Move end pointer backward
    }
}