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
}