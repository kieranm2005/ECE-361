#include "histogram.h"
#include <stdio.h>

void histogram_print(const float a[], int n)
{
    int bins[HISTOGRAM_NUM_BINS] = {0};

    for (int i = 0; i < n; i++) {
        int b = (int) (a[i] / HISTOGRAM_BIN_WIDTH);
        if (a[i] < 0.0f)
            b = 0;
        if (b >= HISTOGRAM_NUM_BINS)
            b = HISTOGRAM_NUM_BINS - 1;
        bins[b]++;
    }

    printf("histogram:\n");
    for (int b = 0; b < HISTOGRAM_NUM_BINS; b++) {
        if (bins[b] == 0)
            continue;
        printf("  %5.1f to %5.1f C | ", b * HISTOGRAM_BIN_WIDTH,
               (b + 1) * HISTOGRAM_BIN_WIDTH);
        for (int k = 0; k < bins[b]; k++)
            putchar('*');
        printf(" %d\n", bins[b]);
    }
}