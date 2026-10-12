#include "reading.h"
#include "stats.h"
#include "histogram.h"

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_THRESHOLD 30.0f

int main(int argc, char *argv[]) {
    float threshold = DEFAULT_THRESHOLD;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }
    int ticks[READING_MAX_READINGS];
    float temps[READING_MAX_READINGS];
    float hums[READING_MAX_READINGS];
    int count = 0;
    int skipped = 0;

    reading_read(stdin, ticks, temps, hums, &count, &skipped);

    printf("readings: %d\n", count);
    printf("skipped:  %d\n", skipped);

    if (count == 0) {
        printf("no readings, no summary\n");
        return 0;
    }

}