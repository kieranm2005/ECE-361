#include "stats.h"

static float max_from(const float a[], int i, int n) {
    if (i == n - 1)
        return a[i];
    float rest = max_from(a, i + 1, n);
}

float stats_min(const float a[], int n) {
    float m = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < m)
            m = a[i];
    }
    return m;
}