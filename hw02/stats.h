#ifndef STATS_H
#define STATS_H

float stats_min(const float a[], int n);

float stats_max(const float a[], int n);

float stats_mean(const float a[], int n);

int stats_longest_run_above(const float a[], int n, float threshold, int *start_out);
#endif