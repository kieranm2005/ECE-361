
#include "reading.h"
#include <stdio.h>

void reading_read(FILE *in,
                  int ticks[], float temps[], float hums[],
                  int *count, int *skipped)
{
    char line[READING_LINE_LEN];
    int  n = 0;
    int  n_skipped = 0;

    while (fgets(line, sizeof line, in) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (n == READING_MAX_READINGS) {
            fprintf(stderr,
                    "warning: more than %d readings, the rest are ignored\n",
                    READING_MAX_READINGS);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[n], &temps[n], &hums[n]) == 3)
            n++;
        else
            n_skipped++;
    }

    *count   = n;
    *skipped = n_skipped;
}