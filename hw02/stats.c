#include "stats.h"

static float max_from(const float a[], int i, int n) {
    if (i == n - 1)
        return a[i];
    float rest = max_from(a, i + 1, n);
    return a[i] > rest ? a[i] : rest;
}

float stats_min(const float a[], int n) {
    float m = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < m)
            m = a[i];
    }
    return m;
}

float stats_max(const float a[], int n) {
    return max_from(a, 0, n);
}

float stats_mean(const float a[], int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return sum / (float) n;
}