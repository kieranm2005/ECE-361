/* tests/test_ptr.c */
#include <stdio.h>
#include <stddef.h>
#include "../ptr.h"

static int failures = 0;
static int total = 0;

static void check(int cond, const char *name)
{
    ++total;
    if (cond) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        ++failures;
    }
}

static int float_eq(float a, float b)
{
    return a == b;
}

static void test_swap_float(void)
{
    float x = 1.5f, y = -2.5f;
    swap_float(&x, &y);
    check(float_eq(x, -2.5f) && float_eq(y, 1.5f), "swap_float basic");
}

static void test_minmax(void)
{
    float mn, mx;
    int r;

    /* several values */
    float a[] = { 3.0f, -1.0f, 7.0f, 0.0f, 5.0f };
    mn = 123.0f; mx = 456.0f;
    r = minmax(a, 5, &mn, &mx);
    check(r == 0 && float_eq(mn, -1.0f) && float_eq(mx, 7.0f),
          "minmax several values");

    /* one value */
    float b[] = { 42.0f };
    mn = 0.0f; mx = 0.0f;
    r = minmax(b, 1, &mn, &mx);
    check(r == 0 && float_eq(mn, 42.0f) && float_eq(mx, 42.0f),
          "minmax one value");

    /* n = 0: outputs unchanged, returns -1 */
    float c[] = { 1.0f, 2.0f, 3.0f };
    mn = 111.0f; mx = 222.0f;
    r = minmax(c, 0, &mn, &mx);
    check(r == -1 && float_eq(mn, 111.0f) && float_eq(mx, 222.0f),
          "minmax n=0 unchanged");

    /* NULL pointer: outputs unchanged, returns -1 */
    mn = 333.0f; mx = 444.0f;
    r = minmax(NULL, 5, &mn, &mx);
    check(r == -1 && float_eq(mn, 333.0f) && float_eq(mx, 444.0f),
          "minmax NULL pointer unchanged");

    /* n < 0 also returns -1 and leaves outputs unchanged */
    mn = 7.0f; mx = 8.0f;
    r = minmax(c, -3, &mn, &mx);
    check(r == -1 && float_eq(mn, 7.0f) && float_eq(mx, 8.0f),
          "minmax n<0 unchanged");
}

static void test_find_above(void)
{
    /* two matches: returns the first */
    float a[] = { 1.0f, 10.0f, 20.0f, 30.0f };
    const float *p = find_above(a, 4, 5.0f);
    check(p == a + 1 && float_eq(*p, 10.0f), "find_above first of two matches");

    /* none */
    float b[] = { 1.0f, 2.0f, 3.0f };
    p = find_above(b, 3, 100.0f);
    check(p == NULL, "find_above none");

    /* equal to threshold is NOT above */
    float c[] = { 5.0f, 6.0f };
    p = find_above(c, 2, 5.0f);
    check(p == c + 1 && float_eq(*p, 6.0f),
          "find_above value equal to threshold skipped");
}

static void test_reverse(void)
{
    /* even length */
    float a[] = { 1.0f, 2.0f, 3.0f, 4.0f };
    reverse(a, 4);
    check(float_eq(a[0], 4.0f) && float_eq(a[1], 3.0f) &&
          float_eq(a[2], 2.0f) && float_eq(a[3], 1.0f),
          "reverse even length");

    /* odd length */
    float b[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    reverse(b, 5);
    check(float_eq(b[0], 5.0f) && float_eq(b[1], 4.0f) &&
          float_eq(b[2], 3.0f) && float_eq(b[3], 2.0f) &&
          float_eq(b[4], 1.0f),
          "reverse odd length");

    /* n < 2: no change */
    float c[] = { 9.0f };
    reverse(c, 1);
    check(float_eq(c[0], 9.0f), "reverse n=1 unchanged");

    /* NULL: no-op (no crash) */
    reverse(NULL, 5);
    check(1, "reverse NULL no-op");
}

int main(void)
{
    test_swap_float();
    test_minmax();
    test_find_above();
    test_reverse();

    printf("\n%d/%d tests passed\n", total - failures, total);
    if (failures != 0) {
        printf("%d test(s) FAILED\n", failures);
        return 1;
    }
    printf("All tests PASSED\n");
    return 0;
}