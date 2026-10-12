#ifndef READING_H
#define READING_H

#include <stdio.h>

#define READING_MAX_READINGS 1000
#define READING_LINE_LEN 128

void reading_read(FILE *in, int ticks[], float temps[], float hums[], int *count, int *skipped);

#endif
