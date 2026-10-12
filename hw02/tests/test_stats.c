/*
 * Unit tests for the stats module. Prints PASS or FAIL for each test and a
 * summary, and exits with a nonzero code if any test fails.
 *
 * Run with:   ./tests/test_stats
 */
#include "../stats.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;
static int passes   = 0;

/* Report one test result and remember it. */
static void check(const char *name, int ok)
{
    if (ok) {
        printf("PASS  %s\n", name);
        passes++;
    } else {
        printf("FAIL  %s\n", name);
        failures++;
    }
}

/* Exact float comparison is fine here: all values are exactly representable
 * or come from the same arithmetic in test and implementation. */
static int feq(float x, float y) { return x == y; }

/* --- Test 1: one value ------------------------------------------------- */
static void test_one_value(void)
{
    float a[1] = { 42.0f };
    int ok = feq(stats_min(a, 1), 42.0f)
          && feq(stats_max(a, 1), 42.0f)
          && feq(stats_mean(a, 1), 42.0f);
    check("one value: min == max == mean == 42.0", ok);
}

/* --- Test 2: maximum at the start and at the end ----------------------- */
static void test_max_at_ends(void)
{
    float start[5] = { 9.0f, 1.0f, 2.0f, 3.0f, 4.0f };
    float end[5]   = { 1.0f, 2.0f, 3.0f, 4.0f, 9.0f };
    int ok = feq(stats_max(start, 5), 9.0f)
          && feq(stats_max(end,   5), 9.0f);
    check("max at start and at end", ok);
}

/* --- Test 3: value exactly equal to the threshold ends a run ----------- */
static void test_threshold_ends_run(void)
{
    /* Threshold 30.0. Readings: 31, 32, 30, 33, 34.
     * Strictly greater than 30: 31, 32 (run 2); 30 is NOT above; then 33, 34
     * (run 2). Longest run is 2, starting at index 0.
     * A second array makes the point sharper: 31, 32, 30, 33.
     * Longest run is 2 (indices 0-1), not 3. */
    float a[5] = { 31.0f, 32.0f, 30.0f, 33.0f, 34.0f };
    int start = -1;
    int len   = stats_longest_run_above(a, 5, 30.0f, &start);
    int ok    = (len == 2) && (start == 0);
    check("threshold-equal value ends the run", ok);

    /* Also check the case where the equal value is the only separator. */
    float b[4] = { 31.0f, 32.0f, 30.0f, 33.0f };
    start = -1;
    len   = stats_longest_run_above(b, 4, 30.0f, &start);
    ok    = (len == 2) && (start == 0);
    check("threshold-equal value splits two runs", ok);
}

/* --- Test 4: recursive maximum on 10,000 values ------------------------ */
static void test_recursive_max_10k(void)
{
    enum { N = 10000 };
    float *a = malloc(N * sizeof *a);
    if (a == NULL) {
        printf("FAIL  recursive max on 10000 values (malloc failed)\n");
        failures++;
        return;
    }
    /* a[i] = (i % 997) - 500, so the maximum is 496, at i = 996, 1993, ... */
    float expected = -1000.0f;
    for (int i = 0; i < N; i++) {
        a[i] = (float) ((i % 997) - 500);
        if (a[i] > expected)
            expected = a[i];
    }
    float got = stats_max(a, N);
    int ok = feq(got, expected) && feq(expected, 496.0f);
    check("recursive max on 10000 values", ok);

    /* Sanity: the same array's min is -500, at i = 500, 1497, ... */
    ok = feq(stats_min(a, N), -500.0f);
    check("min on 10000 values", ok);

    free(a);
}

int main(void)
{
    test_one_value();
    test_max_at_ends();
    test_threshold_ends_run();
    test_recursive_max_10k();

    printf("\n%d passed, %d failed\n", passes, failures);
    return failures == 0 ? 0 : 1;
}