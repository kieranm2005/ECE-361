#include "istack.h"
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;
static int checks   = 0;

#define CHECK(cond, msg)                                            \
    do {                                                            \
        checks++;                                                   \
        if (!(cond)) {                                              \
            printf("FAIL: %s\n", (msg));                            \
            failures++;                                             \
        }                                                           \
    } while (0)

// Verify empty stacks reject pop and peek without modifying the output.
static void test_empty_pop_peek(void)
{
    istack_t *s = istack_create();
    int v = 42;
    CHECK(istack_pop(s, &v) == -1, "pop on empty returns -1");
    CHECK(v == 42, "pop on empty leaves *v alone");
    CHECK(istack_peek(s, &v) == -1, "peek on empty returns -1");
    CHECK(v == 42, "peek on empty leaves *v alone");
    istack_destroy(s);
    printf("PASS: empty pop/peek\n");
}

// Verify pushes and pops follow last-in, first-out ordering.
static void test_lifo(void)
{
    istack_t *s = istack_create();
    istack_push(s, 1);
    istack_push(s, 2);
    istack_push(s, 3);
    int v = 0;
    istack_pop(s, &v); CHECK(v == 3, "LIFO 3");
    istack_pop(s, &v); CHECK(v == 2, "LIFO 2");
    istack_pop(s, &v); CHECK(v == 1, "LIFO 1");
    CHECK(istack_is_empty(s), "empty after pops");
    istack_destroy(s);
    printf("PASS: LIFO\n");
}

// Verify the stack grows at capacity and preserves all stored values.
static void test_growth(void)
{
    istack_t *s = istack_create();
    CHECK(istack_capacity(s) == 8, "starts at 8");
    for (int i = 0; i < 8; i++)
        istack_push(s, i);
    CHECK(istack_capacity(s) == 8, "still 8 at full");
    istack_push(s, 8);
    CHECK(istack_capacity(s) == 16, "doubles to 16 on 9th push");
    for (int i = 0; i < 9; i++) {
        int v;
        istack_peek(s, &v);
        // Verify contents by popping bottom-up later.
        (void)v;
    }
    int arr_expected = 0;
    for (int i = 0; i < 9; i++) {
        int v;
        istack_pop(s, &v);
        CHECK(v == 8 - i, "values preserved after growth");
        (void)arr_expected;
    }
    istack_destroy(s);
    printf("PASS: growth\n");
}

// Verify the stack handles 1,000 values and reports its count correctly.
static void test_1000(void)
{
    istack_t *s = istack_create();
    for (int i = 0; i < 1000; i++)
        CHECK(istack_push(s, i) == 0, "push 1000");
    CHECK(istack_count(s) == 1000, "count 1000");
    for (int i = 999; i >= 0; i--) {
        int v;
        istack_pop(s, &v);
        CHECK(v == i, "pop 1000 in order");
    }
    CHECK(istack_is_empty(s), "empty after 1000 pops");
    istack_destroy(s);
    printf("PASS: 1000 pushes/pops\n");
}

// Verify conversion returns bottom-first contents in an independent copy.
static void test_to_array(void)
{
    istack_t *s = istack_create();
    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);

    size_t n = 0;
    int *a = istack_to_array(s, &n);
    CHECK(a != NULL, "to_array non-NULL");
    CHECK(n == 3, "to_array n==3");
    CHECK(a[0] == 10 && a[1] == 20 && a[2] == 30, "bottom-first order");

    // Copy: mutating a does not affect the stack.
    a[0] = 999;
    int v;
    istack_peek(s, &v);
    CHECK(v == 30, "stack unaffected by array mutation");
    free(a);
    istack_destroy(s);
    printf("PASS: to_array\n");
}

// Verify all public operations handle NULL inputs safely and consistently.
static void test_null(void)
{
    int v = 7;
    CHECK(istack_push(NULL, 1) == -1, "push NULL");
    CHECK(istack_pop(NULL, &v) == -1 && v == 7, "pop NULL");
    CHECK(istack_peek(NULL, &v) == -1 && v == 7, "peek NULL");
    CHECK(istack_count(NULL) == 0, "count NULL");
    CHECK(istack_is_empty(NULL), "is_empty NULL");
    CHECK(istack_capacity(NULL) == 0, "capacity NULL");
    size_t n = 5;
    CHECK(istack_to_array(NULL, &n) == NULL && n == 0, "to_array NULL");
    istack_destroy(NULL);  // Must not crash.
    printf("PASS: NULL handling\n");
}

// Verify separate stacks do not share contents or lifetime.
static void test_independent(void)
{
    istack_t *a = istack_create();
    istack_t *b = istack_create();
    istack_push(a, 1);
    istack_push(b, 2);
    int v;
    istack_peek(a, &v); CHECK(v == 1, "stack a independent");
    istack_peek(b, &v); CHECK(v == 2, "stack b independent");
    istack_destroy(a);
    istack_peek(b, &v); CHECK(v == 2, "b survives a destroyed");
    istack_destroy(b);
    printf("PASS: independence\n");
}

// Verify destroying a populated stack completes without leaking or crashing.
static void test_destroy_nonempty(void)
{
    istack_t *s = istack_create();
    for (int i = 0; i < 100; i++)
        istack_push(s, i);
    istack_destroy(s);
    printf("PASS: destroy non-empty\n");
}

int main(void)
{
    test_empty_pop_peek();
    test_lifo();
    test_growth();
    test_1000();
    test_to_array();
    test_null();
    test_independent();
    test_destroy_nonempty();

    printf("\n%d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}